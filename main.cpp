#include <iostream>
#include <QThread>
#include <QDebug>
#include <vector>
#include <QImage>

#include "rectifythread.h"
#include "correctionfactor.h"


using namespace std;

int main(int argc, char *argv[]){
    if(argc < 2){
        cout << "Usage: meteor_rectify <input file>" << endl;
        return 1;
    }
    const string out_fname = static_cast<string>(argv[1]).substr(0, static_cast<string>(argv[1]).length() - 4) + "-rectified.png";
    QImage originalImage(argv[1]);
    if(originalImage.isNull()){
        cout << "Unable to load image." << endl;
        return 2;
    }
    unsigned int width = originalImage.width();
    unsigned int height = originalImage.height();
    vector<unique_ptr<RectifyThread>> workers;
    int number_threads = 4;
    int worker_rows, start_row, end_row;
    start_row = 0;
    CorrectionFactor correctionFactors(width);
    QImage rectifiedImage(correctionFactors.getRectifiedWidth(), originalImage.height(), originalImage.format());

    //Calculate some starting parameters
    worker_rows = ceil(height / number_threads);
    end_row = worker_rows;

    //Create threads
    do{
        workers.push_back(make_unique<RectifyThread>(originalImage, &rectifiedImage, correctionFactors.getRectifiedWidth(), correctionFactors.getVector(), end_row, start_row));
        cout << "Creating thread " << workers.size() << endl;
        start_row = worker_rows * static_cast<int>(workers.size());
        end_row = start_row + worker_rows;
    }while(static_cast<int>(workers.size()) < number_threads);

    //Start threads
    for(int workerNumber = 0; workerNumber < number_threads; workerNumber++){
        cout << "Spawning thread " << workerNumber + 1 << endl;
        workers[workerNumber]->start();
    }

    //Wait on them to finish
    for(int workerNumber = 0; workerNumber < number_threads; workerNumber++){
        workers[workerNumber]->wait();
        cout << "Thread " << workerNumber + 1 << " finished." << endl;
    }

    //Save the output
    cout << "Writing rectified image to " << out_fname << endl;
    rectifiedImage.save(QString::fromStdString(out_fname));
}

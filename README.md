#Beeldherkenning
Door Koen Severijns

### Build
    mkdir build
    cd build
    cmake ..
    make

### Run
    cd build
    ./opencv-project -mode=(i voor interactief/b voor batch) -file=../batch.txt(of een andere batch file) -webcam=(-1 voor standaard img/webcam ID)
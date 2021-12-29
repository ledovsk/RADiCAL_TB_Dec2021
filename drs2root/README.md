Convert DRS raw file without calibration into calibrated ROOT file.   
Calibration is provided by CAEN and is stored inside DT5742 module

To compile

``` 
make clean    
make   
```

To process one run:  
```
./maketree --inputFileName=<your_caen_drs4_file.dat> --outputFileName=<your_output_file.root> 
```   

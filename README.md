# Documentation (Slovak language)
Program documentation (Programová dokumentácia)                                                                                                  
https://github.com/MiroslavKohut/quadrocopter_driver/blob/master/Documentation/important/Programova_dokumentacia.pdf
User documentation (Užívateľská dokumentácia)                                                                                       
https://github.com/MiroslavKohut/quadrocopter_driver/blob/master/Documentation/important/Uzivatelska_dokumentacia_k_zaver_projektu_kvadrokoptera.pdf

# quadrocopter_driver
This repository contains experimental code for PID quadrocopter control using MPU9250 and radio control. Yaw axis is tested and calibrated.
Pitch and roll axis are prepared for PID calibration. For yaw axis control we are using P regulator with raw data from gyroscope filtered with Moveing average filter.
We also implemented complementary filter for pitch and roll rotation. Filtering of data works fine but Controller still needs a better calibration and testing.
We do not suggest to edit the code. Only at your own risk.

# VIDEOS
YAW stabilization                                                                                                        
https://www.youtube.com/watch?v=hHbUFp_3BBc                                                                     
PITCH AND ROLL stabilization testing                                                                                       
https://www.youtube.com/watch?v=z78SoeCwCng

# Institute
Slovak University of Technology in Bratislava (STU)                                                                 
Faculty of Electrical Engineering and Information Technology                                                            
Institute of Robotics and Cybernetics (ÚRK)

# LICENCE
The MIT License (MIT)
Copyright (c) 2009-2016 Atollic AB

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

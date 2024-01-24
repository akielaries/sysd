# sysd
> :warning: In active development along with [liblcd](https://github.com/akielaries/benchmarks) and [libinfluxdb](https://github.com/akielaries/benchmarks)

System Daemon for Real-Time Monitoring

Lots of the code from [benchmarks](https://github.com/akielaries/benchmarks) can be included into this. 

Main focus:
1. collect system information
2. send information to central DB (influx or maria)
3. visualize with grafana from DB connector
4. display on connected I2C display on devices GPIOs (KiCAD, circuit design, and some protoboard soldering will be needed)

The use case here is my home network/lab setup with a plethora of single board computers (Jetsons, RPis, beaglebones, MCUs, etc) that I want to monitor system info live such as VRAM, RAM, CPU temp, usage, processes, etc, etc

# Install
Requires: 
* [liblcd](https://github.com/akielaries/liblcd) for I2C LCD interaction
* Make
* gcc
* c99

Compile with `make`

# Usage
sysd is CLI-based by nature with 3 main modes.
* Default (simple monitoring and logging)
* Publish (monitoring and data transmission to central host)
* Subscribe (collect from publisher)

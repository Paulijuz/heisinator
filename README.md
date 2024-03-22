# Heisinator
Heisinator is a simple elevator controller developed as part of the coursework for TTK4235 Embedded Systems at NTNU. It is written in C and makes use of the custom elevator control library `elevio` used in the course. In addition this project also includes the unit test framework `Unity`. The source of the framework can be found [here](https://github.com/ThrowTheSwitch/Unity).

## General
To compile Heisinator:
```bash
make
```

To run Heisinator:
```bash
./elevator
```

## Doxygen
This project features documentaion written for doxygen. To generate the Doxygen documentation, run the following command in the root directory of the project:
```bash
make docs
```

## Tests
To compile the unit tests:
```bash
make test
```

To run the tests:
```bash
./unit_tests
```

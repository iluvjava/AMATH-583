### This file is used for parsing the data which we logged for different 
### Parallelization strategies. 
### Used for AMATH 583 SPRING 2021 PS5 
### Author: Hongda Alto Li
import numpy as np
import pandas as pd

def ReadData(fname:str): 
    DataRows = []
    with open(fname) as File: 
        Lines = File.readlines()
        for line in Lines:  
            Tokens = line.split()
            if len(Tokens) > 0 and Tokens[0].isnumeric():
                DataRows.append(Tokens[: int((len(Tokens) - 2)/2) + 2])
    for II in range(len(DataRows)): 
        DataRows[II] = list(map(float, DataRows[II]))
    return DataRows


def ComputeSpeedUp(DataRows): 
    DataRows = np.array(DataRows)
    Flops = 2*DataRows[:,[0]]
    FlopsPerSec =  1e9*DataRows[:, 1::]
    Time = Flops/FlopsPerSec
    SpeedUp = Time[:, [1]]/Time
    SpeedUpWithSize = np.zeros((SpeedUp.shape[0], SpeedUp.shape[1] + 1))
    SpeedUpWithSize[:, 0] = DataRows[:, 0]
    SpeedUpWithSize[:, 1:] = SpeedUp
    return SpeedUpWithSize


def SpeedUp2Latex(Speedup):
    Data = pd.DataFrame(data = Speedup)
    return Data.to_latex()
    

def main():
    DataRead = ReadData("pnorm_small.log")
    print("This is the data read")
    print(DataRead)
    print("This is the speed up: ")
    Speedup = ComputeSpeedUp(DataRead)
    print(Speedup)
    print("This is the latex")
    print(SpeedUp2Latex(Speedup))
    


if __name__ == "__main__": 
    import os 
    print(f"cwd: {os.getcwd()}")
    main()
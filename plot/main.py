###############################################################################
# Evolutionary Adaptive Trophic Structure Model 2 (EATSM2)                    #
#                                                                             #
# (C) Copyright 2024, Phil Underwood. All rights reserved.                    #
#                                                                             #
# This software is licensed under the terms of the 3-Clause BSD License       #
# which can be obtained from https://opensource.org/license/bsd-3-clause/.    #
###############################################################################

from os.path import exists

import matplotlib.pyplot as plt
import netCDF4 as nc
from plot_utils import area_like_matlab

###############################################################################
## PARAMS
###############################################################################
data_dir = "/home/doomsayer/Development/Repositories/eatsm2/build/Debug/output/"
data_file = "2026-01-25_18:40:30.nc"

var_names_totals = ["totalHeterotrophFrequency"]

name_data_size = "dataSize"
name_number_size_classes = "numberOfSizeClasses"

###############################################################################
file_path = data_dir + data_file

if exists(file_path) == 1:
    
    print('-------------------------------------------------------------------------------')
    print(file_path, "exists...")
    data_set = nc.Dataset(file_path, format="NETCDF4")
        
    #sz_data_size = data_set.dimensions[name_data_size]
    #sz_number_size_classes = data_set.dimensions[name_number_size_classes]
  
    for group in data_set.groups:
        print("Getting group data for", group)
        variables = data_set.groups[group].variables
        
        # Create volume histogram
        vol_nutrient = variables["nutrientVolume"][:]
        vol_autotrophs = variables["autotrophVolume"][:]
        vol_heterotrophs = variables["heterotrophVolume"][:]
        
        ax, polys = area_like_matlab(
            [vol_nutrient, vol_autotrophs, vol_heterotrophs],
            labels=['Nutrient', 'Autotrophs', 'Heterotrophs']
        )
        plt.show()
        
        freq_heterotrophs = variables["heterotrophFrequency"][:]
        plt.plot(freq_heterotrophs)
        plt.xlabel('time')
        plt.ylabel('heterotrophFrequency')
        
        #for var_name in variables:
        #    print("Plotting for", var_name)
        #    var_data = variables[var_name][:]
        #    
        #    plt.plot(var_data)
        #    plt.xlabel('time')
        #    plt.ylabel(var_name)
            
    data_set.close()
    
    ###########################################################################
else:
    print(file_path, "does not exist...")
  
###############################################################################
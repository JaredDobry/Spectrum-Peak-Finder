# Spectrum-Peak-Finder
Code designed to pick out peaks in spectrography data, given a manually entered cutoff value.
Data must be placed in the format as follows:
at C:/Data/DataIn.txt
in the format
Y-value(tab key)X-value
so that the C++ code can interpret that tab key as the \t character and parse the data into the datamap.
Data will be spat out into the same folder, in new files, one for each set of peak data
C:/Data/Peak0.txt etc

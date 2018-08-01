/*
 * Copyright (c) 2018 A.C. Kockx, All Rights Reserved.
 */

#include "util/FileUtils.h"

#include <fstream>
#include <string>

string readFile(string filePathName) {
    ifstream input;
    input.open(filePathName, ios::in);

    string line;
    string lines;
    while (getline(input, line)) {
        lines += line + "\n";
    }

    input.close();
    return lines;
}

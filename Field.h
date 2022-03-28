#include <algorithm>
#include <vector>
#include <iostream>
#include <string>
#pragma once

class Field
{
    public:
    Field(const std::vector<std::vector<int>> & Field_Vector);   //Copy Constructor
    Field(std::vector<std::vector<int>> && Field_Vector);    //Move Assignment
    int Weight(int x1, int y1, int x2, int y2); //function to calculate the 
    int PathCost(); //Function to calculate lowest cost to traverse from left to right

    private:
    std::vector <std::vector<int>> Stored_Path; //The first vector  
    std::vector <std::vector<int>> Weight_Holder; // The second vector 
};

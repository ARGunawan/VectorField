#include <vector>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <iostream>
#include "Field.h"

//Copy Constructor
//Since we do not need to initalize the default constructor, we need to initalize everything within the copy constructor
//The computation here will then be moved into the move constructor
 Field::Field(const std::vector<std::vector<int>> &Field_Vector)
 {
     Stored_Path = Field_Vector; //We store our parameter vector Field_Vector into our vector Stored_Path to have it defined within the constructor
     int fv_row = Field_Vector.size(); //We initialize the rows by taking the size of our parameter vector as it is now equal to our Stored_Path
     int fv_column = Field_Vector[0].size(); //We do the same as the above for column, but place a bracket [0], before size as a row is in a sense a column 

     //We utilize the resize member for changing of the sizes of our vector Weight_Holder
     //In this case, we have it so that it stores the rows, and takes the vector of the columns
     //This allows it to change the content of our vector through either erasing or inserting the elements inside of it
     Weight_Holder.resize(fv_row, std::vector<int>(fv_column)); 

    //Create our for loops to search through the rows and columns of our vector
     for(int i = 0; i < fv_row; i++)
     {
         for(int j = 0; j < fv_column; j++)
         {
            if(i >= 0 && j >= 0) //Assuming i and j are greater than or equal to 0
            {
                if (i == 0 && j == 0) //Create another condition if i and j are 0 or have no values
                {
                    Weight_Holder[i][j] = Field_Vector[i][j]; //Sett it equal to our parameter vector as there is nothing inside
                }
                 else if(j == 0) //Create the edge case for the columns if it is 0
                {
                    Weight_Holder[i][j] = Weight_Holder[i - 1][j] + Field_Vector[i][j];
                }
                else if(i == 0) //Create the edge case for the row if it is 0
                {
                    Weight_Holder[i][j] = Weight_Holder[i][j - 1] + Field_Vector[i][j];
                }
                else //Otherwise if none of the above conditions are checked for, this will check for the remainder of our vector
                {
                    Weight_Holder[i][j] = (Weight_Holder[i-1][j] + Weight_Holder[i][j-1] + Field_Vector[i][j]) - Weight_Holder[i-1][j-1];
                }
            }
        
        }   
    } 
} 
 

//Move Constructor
//We will move our copy constructor into the move constructor
//Because std::move will erase our parameter vector from copy constructor, we must set Stored_Path to it once more
//This will prevent compiling and referencing errors from occurring
Field::Field(std::vector<std::vector<int>> &&Field_Vector)
{
     Stored_Path = std::move(Field_Vector); //Have our vector set equal to the move member function and store our parameter vector inside
     int fv_row = Stored_Path.size(); //This time set our vector to be inside our row. 
     int fv_column = Stored_Path[0].size();//We do the same as the above.

     //We keep the same variable as the above since we are moving our logic into the move constructor
     Weight_Holder.resize(fv_row, std::vector<int>(fv_column));

    //Utilizing the same logic and for loop for testing
    //The only exception this time is that isntead of our parameter vector being test, we have it set to our vector
     for(int i = 0; i < fv_row; i++)
     {
         for(int j = 0; j < fv_column; j++)
         {
            if(i >= 0 && j >= 0) //Same conditions as the above constructor
            {
                if (i == 0 && j == 0) //Same as the above conditions as the constructor
                {
                    Weight_Holder[i][j] = Stored_Path[i][j]; //Change to our vector to prevent referencing errors
                } 
                 else if(j == 0)
                {
                    Weight_Holder[i][j] = Weight_Holder[i - 1][j] + Stored_Path[i][j]; //Same logic as the previous 
                }
                else if(i == 0)
                {
                    Weight_Holder[i][j] = Weight_Holder[i][j - 1] + Stored_Path[i][j]; //Same logic as the previous 
                }
               
                else //Else check for the remainder of our vector
                {
                    Weight_Holder[i][j] = (Weight_Holder[i-1][j] + Weight_Holder[i][j-1] + Stored_Path[i][j]) - Weight_Holder[i-1][j-1];
                }
            }
        }
    }
} 
 
//The Weight Function
//The weight function will be where our edge cases for certain situations that might occur within our vector 
//when searching at the indexes of matrix
int Field::Weight(int x1,int y1,int x2,int y2) 
{
    //Instead of Stored_Path, we set these to Weight_Holder as it now old the computation needed to be done for the following edge cases
   int fv_row = Weight_Holder.size(); //We have our row set to our other vector Weight_Holder instead as it now olds the computation of the elements
   int fv_column =  Weight_Holder[0].size(); //Same reasoning as above 

   //These following variables are needed to search through the matrix to obtain the below edge cases

   //Note: These values are swapped to get the indexes of our values within the matrix.  
   int left_corner_x = std::min(y1, y2); // 1) This will be used to gain the upper left corner index of our matrix. Set to gain the min
   int right_corner_x = std::max(y1, y2); // 2) This will be used to gain the bottom right of our matrix. Set to max
   int left_corner_y = std::min(x1, x2); // 3) This will be also used to get the upper left. Set to min 
   int right_corner_y = std::max(x1, x2); // 4) This will be also used to get the bottom right. Set to max
   
   //Have the variable sum set to int and then store the value of when both bottom right corners are 0
   //It is initialized here so that our variable has the stored vector inside of it
   //This will our the following edge cases to be computed more easily
   int sum = 0;
   sum = Weight_Holder[right_corner_x][right_corner_y];
  
   //If the row and columns for each of these values are all greater than zero proceed.
   //Note: Doing  fv_row > x1 && x2 >= 0 and doing the same for column will not always work and may cause an error when compiling
   if((fv_row > x1 >= 0) && (fv_row > x2 >= 0) && (fv_column > y1 >= 0) && (fv_column > y2 >= 0))
    {   
        if(left_corner_x >= 1) //If the left corner is greater than or equal to 1
        {
            //Set the sum so that it is being subtracted by our vector for the upper left corner to check
            sum = sum - Weight_Holder[left_corner_x-1][right_corner_y];
        }
        if(left_corner_y >= 1)
        {
            //Same reasoning as the above, but this time for our y
            sum = sum  - Weight_Holder[right_corner_x][left_corner_y-1];
        }
        //Checks for both the bottom right corners in the event that they are in fact empty or 0
        if(right_corner_x == 0 && right_corner_y == 0)
        {
            //We will just return our store variable sum here
            return sum;
        }
        //Just like the above we will check for the upper left index values. This time we set it to it in the event that they are greater than or equal to 1
        if(left_corner_x >= 1 && left_corner_y >= 1)
        {
            //The sum now takes the difference of our the upper left for our vector Weight_Holder
            sum = sum + Weight_Holder[left_corner_x-1][left_corner_y-1];
        }
        //In the possible event that there is absolutely nothing in our vector at all, just return 0
        //This is because there are no elements within it, so there is nothing to compute
        if(Weight_Holder.empty())
        {
            return 0;
        }
    }
    //If all else fails, then we create our throw condition, to showcase that the particular value being checked for is completely out of bounds
    else
    {
        throw std::out_of_range("This value is out of range/bounds!");
    } 
    //Return the sum here so that the value of our vector is returned
    return sum;
}


//The Path Cost Function
//The purpose of this function is to check for the lowest cost path to search through our vector
//This will be so that we can check for the most cost efficient path to take 
int Field::PathCost()
{   
    int fv_row =  Weight_Holder.size(); //Once again like our weight function, we must utilize our vector from the copy constructor since this holds the computation 
    int fv_column = Weight_Holder[0].size(); //Same reasoning as the above.

    //We now create a new vector called lowest_cost
    //This vector will store the information of the lowest cost for traveling through our matrix
    std::vector <std::vector<int>> lowest_cost; 

    //Again like our copy constructor and move constructor, we need to set the new vector to be resized
    //This will edit the elements contained inside of our vector lowest_cost
    lowest_cost.resize(fv_row, std::vector<int> (fv_column));

    //Once again create a for loop to search through our edge cases
    //The edge cases are in place to check for the following conditions if these are met 
    for(int i = 0; i < fv_row; i++)
    {
        for(int j = 0; j < fv_column;j++)
        {
            //Just like our copy constructor this will test if either the row and columns happen to be zero
            //If so, just set lowest_cost to our Stored_Path vector
            if(i == 0 && j == 0)
            {
                lowest_cost[i][j] = Stored_Path[i][j];
            }
            //Will check for the columns 
            else if(j == 0)
            {
                lowest_cost[i][j] = Stored_Path[i][j] + lowest_cost[i-1][j];
            }
            //Will check for the rows
            else if(i == 0)
            {
                lowest_cost[i][j] = Stored_Path[i][j] + lowest_cost[i][j-1];
            }
            //Otherwise if the other edge cases are not met, this will check for the remainder path costs for our matrix
            //Note: We take the minimum of the vectors because we are looking for the lowest cost to travel from upper left to the bottom right of our vector
            else
            {
                lowest_cost[i][j] = Stored_Path[i][j] + std::min(lowest_cost[i-1][j],lowest_cost[i][j-1]);
            } 
        }
    }
    //Return the vector and do the differences 
    return lowest_cost[fv_row - 1][fv_column - 1];
}

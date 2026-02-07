//65206509 Zarene Jonker
#include <iostream>

#include <deque> /*I need to include this header file in order to work with a deque.
I chose to use a deque because a deque allows fast random access to any element. It also allows efficient
insertion and deletion at both ends of the container*/

#include <fstream> //Data about the traffic flow is to be read from a text file, so the <fstream> header file has to be included.

#include <string> //Needed for working with strings
#include <cctype> //Needed for working with characters
#include <vector> //Needed for working with vectors
#include <iomanip> //Needed for manipulating output

#include <cmath> /*Needed for calculating sqaure roots
(such as when calculating deviants that we need for determining categories of congestion levels)*/

using namespace std;

/*I am using a struct to keep all relevant infomation about a vehicle together.
I am using a struct instead of a class, since no member functions are needed.*/
struct Vehicle
{
    string timeStamp, vehicleType;
    int speed;
};

//Function to display the header
void displayHeader()
{
    cout << "Traffic Flow Analyzer" << endl;
    cout << "---------------------" << endl;
}

//Peak Traffic Times Report
/*I chose the Peak Traffic Times Report because it reflects the hours, day and time intervals with the highest traffic volumes.
Highest traffic volumes represent a maximum volume that the road has to handle and can be a good indication of whether it currently
can handle the volume of traffic or whether it requires expansion.
*/

//Function to get the overall peak time
void overallPeakTime(deque <Vehicle> & trafficFlowP, ostream & outStreamP)
{
    //I used a vector to keep track of the number of vehicles per hour. Each element represents an hour.
    vector <int> hours(24);
    for (int i = 0; i < static_cast<int>(trafficFlowP.size()); i++) //Use a loop to iterate through the elements of the deque
    {
        string hour = trafficFlowP[i].timeStamp.substr(11,2); //Retrieve the hour from the string
        int hr = std::stoi(hour); //Convert the hour to an integer so that it can be compared with the required hours
        hours[hr]++; //The corresponding element in the vector is updated.
    }

    //Get the hour with the highest total number of vehicles
    int highest = 0; //Variable to store the highest number of vehicles
    int startHour, endHour; //Variables to store the hour which had the highest number of vehicles
    for(int i = 0; i < 24; i++)
    {
        if(hours[i] > highest) //Check whether the current element is higher than the highest number of vehicles
        {
            highest = hours[i]; //If the current element is higher than the highest number of vehicles, make that element the new highest number
            startHour = i; //Set the hour which had the highest number of vehicles to the hour represented by the current element
            if(startHour == 23) //Ensure that the hour "23:00 - 00:00" is output in the correct format
            {
                endHour = 0; //Set the endHour variable to 00 instead of 24
            }
            else
                {
                    endHour = startHour + 1;
                }
        }
    }

    //Output the overall peak traffic time
    outStreamP << "Peak traffic time (overall): " << setw(2) << setfill('0') << startHour << ":00 - " << setw(2) << setfill('0') << endHour << ":00 with " << highest << " vehicles" << endl;
}

//Function to get the peak hour of each day
void processDay(deque <Vehicle> & trafficFlowP, int & lastDayCheckedP, int dayNumberP, ostream & outStreamP)
/*We will need to know the position of the last element before moving on to the next day.
The lastDaycheckedP parameter helps with this*/
{
    vector <string> dayData; //Use a vector to keep track of all the data for the day
    vector <int> hours(24); //We will use a vector to keep track of the hours in each day

    for (int i = lastDayCheckedP; i < static_cast<int>(trafficFlowP.size()); i++) //The for loop iterates through the deque so that we can compare values
    {
        string d = trafficFlowP[i].timeStamp.substr(8, 2); //Retrieve the day from the string
        int day = std::stoi(d); //Convert the day to an integer so that it can be compared with other days

        if (i > 0 && i != lastDayCheckedP) //If this is not the first element, we can check to see if this is still the same day
        {
            string previousD = trafficFlowP[i - 1].timeStamp.substr(8, 2); //Retrieve the day from the previous string
            int previousDay = stoi(previousD); //Convert the day to an integer so that it can be compared with other days
            if (day == previousDay) //If this is still the same day, we can add this element to the vector for the day
            {
                dayData.push_back(trafficFlowP[i].timeStamp); //We only need to add the time stamp to the vector, so that we can check the hours
            }
            else
                {
                    lastDayCheckedP = i; //Set the variable to the last element that was checked so that we know from where to continue with the next day
                    break; //If this is not the same day anymore, the loop is ended
                }
        }
        else
            {
                dayData.push_back(trafficFlowP[i].timeStamp); //We need to ensure that the first element is also processed
            }

    }

    //Count the number of vehicles for each hour of the day
    for (int i = 0; i < static_cast<int>(dayData.size()); i++) //Use a loop to iterate through the elements of the vector
    {
        string hour = dayData[i].substr(11,2); //Retrieve the hour from the string
        int hr = std::stoi(hour); //Convert the hour to an integer so that it can be compared with the required hours
        hours[hr]++; //The corresponding element in the vector is updated.
    }

    //Get the hour with the highest number of vehicles
    int highest = 0; //Variable to store the highest number of vehicles
    int startHour, endHour; //Variables to store the hour which had the highest number of vehicles
    for (int i = 0; i < 24; i++)
    {
        if (hours[i] > highest) //Check whether the current element is higher than the highest number of vehicles
        {
            highest = hours[i]; //If the current element is higher than the highest number of vehicles, make that element the new highest number
            startHour = i; //Set the hour which had the highest number of vehicles to the hour represented by the current element
            if(startHour == 23) //Ensure that the hour "23:00 - 00:00" is output in the correct format
            {
                endHour = 0; //Set the endHour variable to 00 instead of 24
            }
            else
                {
                    endHour = startHour + 1;
                }
        }
    }

    // Display the result
    outStreamP << "Day " << dayNumberP << ": " << setw(2) << setfill('0') << startHour << ":00 - " << setw(2) << setfill('0') << endHour << ":00, " << setw(2) << setfill(' ') << highest << " vehicles" << endl;
}

//Function to get the peak traffic by day
void peakTrafficByDay(deque <Vehicle> & trafficFlowP, ostream & outStreamP)
{
    int lastDayChecked = 0; //Variable to keep track of the position of the last element before moving on to the next day

    for (int dayNumber = 1; dayNumber <= 6; ++dayNumber) //We will use a for loop to display the results for 6 days
    {
        processDay(trafficFlowP, lastDayChecked, dayNumber, outStreamP); //Use the processDay function to display the peak traffic by day
    }
}

//Function to display the hourly peak traffic
void processHourlyData(deque <Vehicle> & trafficFlowP, int hourNumberP, ostream & outStreamP)
{
    vector <int> days(6, 0); //Use a vector to keep track of all the data for the hour

    for (int i = 0; i < static_cast<int>(trafficFlowP.size()); i++) //The for loop iterates through the deque so that we can compare values
    {
        string hr = trafficFlowP[i].timeStamp.substr(11, 2); //Retrieve the hour from the string
        int hour = std::stoi(hr); //Convert the hour to an integer so that it can be compared with other hours

        if (hour == hourNumberP) //Check if this is the specified hour
        {
            string d = trafficFlowP[i].timeStamp.substr(8, 2); //Retrieve the day from the string
            int day = stoi(d); //Convert the day to an integer

            if (day >= 1 && day <= 6) //Ensure the day is within the valid range (Day 1 to Day 6)
            {
                days[day - 1]++; //Increment the count for the corresponding day
            }
        }
    }

    //Find the day with the highest number of vehicles
    int highest = 0; //Variable to store the highest number of vehicles
    int highestDay = -1; //Variable to store the day with the highest number of vehicles
    for (int i = 0; i < 6; i++)
    {
        if (days[i] > highest) //Check whether the current element is higher than the highest number of vehicles
        {
            highest = days[i]; //If the current element is higher than the highest number of vehicles, make that element the new highest number
            highestDay = i + 1; //Set the day which had the highest number of vehicles to the day represented by the current element
        }
    }

    // Display the result
    int endHour;
    if(hourNumberP == 23) //Ensure that the hour "23:00 - 00:00" is output in the correct format
        {
            endHour = 0; //Set the endHour variable to 00 instead of 24
        }
        else
            {
                endHour = hourNumberP + 1;
            }

    outStreamP << setw(2) << setfill('0') << hourNumberP << ":00 - " << setw(2) << setfill('0') << endHour << ":00 : Day " << highestDay << ", " << setw(2) << setfill(' ') << highest << " vehicles" << endl;
}

//Function to get the hourly peak traffic
void hourlyPeakTraffic(deque <Vehicle> & trafficFlowP, ostream & outStreamP)
{
    for (int hourNumber = 0; hourNumber < 24; ++hourNumber) //We will use a for loop to display the results for every hour of the day
    {
        processHourlyData(trafficFlowP, hourNumber, outStreamP); //Use the processHourlyData function to display the hourly peak traffic
    }
}

//Function to get the day that had the most traffic
void getDayWithHighestTrafficVolume(deque <Vehicle> & trafficFlowP, ostream & outStreamP)
{
    int lastDayChecked = 0;
    /*We will need to know the position of the last element before moving on to the next day.
    The lastDaychecked variable helps with this*/

    vector <int> days(6); //We will use a vector to keep track of the number of vehicles for each day. The elements represent the days
    int daysIndex  = 0; //The daysIndex helps keep track of which day we are updating

    for (int i = lastDayChecked; i < static_cast<int>(trafficFlowP.size()); i++) //The for loop iterates through the deque so that we can compare values
    {
        string d = trafficFlowP[i].timeStamp.substr(8, 2); //Retrieve the day from the string
        int day = std::stoi(d); //Convert the day to an integer so that it can be compared with other days

        if (i > 0 && i != lastDayChecked) //If this is not the first element, we can check to see if this is still the same day
        {
            string previousD = trafficFlowP[i - 1].timeStamp.substr(8, 2); //Retrieve the day from the previous string
            int previousDay = stoi(previousD); //Convert the day to an integer so that it can be compared with other days
            if (day == previousDay) //If this is still the same day, we can increment the amount of vehicles for that day
            {
                days[daysIndex]++; //Increase the number of vehicles for the day
            }
            else
                {
                    lastDayChecked = i; //Set the variable to the last element that was checked so that we know from where to continue with the next day
                    daysIndex++; //Move the index to the next element of the vector as we move on to the next day
                    break; //If this is not the same day anymore, the loop is ended
                }
        }
        else
            {
                days[daysIndex]++; //We need to ensure that the first element is also processed
            }

    }

    //Get the day with the highest number of vehicles
    int highestNumberOfVehicles = 0; //Variable to store the highest number of vehicles
    int dayNumber = 0; //Variable to store the day
    for (int i = 0; i < 6; i++)
    {
        if (days[i] > highestNumberOfVehicles) //Check whether the current element is higher than the highest number of vehicles
        {
            highestNumberOfVehicles = days[i]; //If the current element is higher than the highest number of vehicles, make that element the new highest number
            dayNumber = i + 1; //Set the day which had the highest number of vehicles
        }
    }

    // Display the result
    outStreamP << "The day that had the highest traffic volume was Day " << dayNumber << " with " << highestNumberOfVehicles << " vehicles" << endl;
}

//Function for Peak Traffic Times Report
void peakTrafficTimesReport(deque <Vehicle> & trafficFlowP, ostream & outStreamP)
{
    char choice; //Variable to store the option the user selected
    bool validChoice = false; //Variable to check whether a valid option was selected
    cout << "Please select the part of the report to generate: " << endl;
    cout << "Full Report (F)" << endl << "Overall Peak Time (O)" << endl << "Peak Traffic By Day (D)" << endl << "Peak Traffic By Hour (H)" << endl << "Day With The Highest Traffic Volume (V)" << endl;

    while(cin >> choice)
    {
        //I am using a switch to allow customization for generating the report
        switch(choice)
        {
        case 'f':
        case 'F':
            //Display the header for the report
            outStreamP << "----------------------------------------------------------------" << endl;
            outStreamP << "Peak Traffic Times Report" << endl;
            outStreamP << "----------------------------------------------------------------"<< endl << endl;

            //Display the overall peak traffic time
            overallPeakTime(trafficFlowP, outStreamP);
            outStreamP << endl;

            //Display peak traffic by day
            outStreamP << "Peak traffic by day:" << endl;
            peakTrafficByDay(trafficFlowP, outStreamP);
            outStreamP << endl;

            //Display the hourly peak traffic
            outStreamP << "Hourly peak traffic:" << endl;
            hourlyPeakTraffic(trafficFlowP, outStreamP);
            outStreamP << endl;

            //Determine which day had the highest traffic volume
            getDayWithHighestTrafficVolume(trafficFlowP, outStreamP);

            cout << "Report saved." << endl;
            validChoice = true;
            break;
        case 'o':
        case 'O':
            //Display the header for the report
            outStreamP << "----------------------------------------------------------------" << endl;
            outStreamP << "Peak Traffic Times Report" << endl;
            outStreamP << "----------------------------------------------------------------" << endl << endl;

            //Display the overall peak traffic time
            overallPeakTime(trafficFlowP, outStreamP);
            outStreamP << endl;

            cout << "Report saved." << endl;
            validChoice = true;
            break;
        case 'd':
        case 'D':
            //Display the header for the report
            outStreamP << "----------------------------------------------------------------" << endl;
            outStreamP << "Peak Traffic Times Report" << endl;
            outStreamP << "----------------------------------------------------------------" << endl << endl;

            //Display peak traffic by day
            outStreamP << "Peak traffic by day:" << endl;
            peakTrafficByDay(trafficFlowP, outStreamP);
            outStreamP << endl;

            cout << "Report saved." << endl;
            validChoice = true;
            break;
        case 'h':
        case 'H':
            //Display the header for the report
            outStreamP << "----------------------------------------------------------------" << endl;
            outStreamP << "Peak Traffic Times Report" << endl;
            outStreamP << "----------------------------------------------------------------" << endl << endl;

            //Display the hourly peak traffic
            outStreamP << "Hourly peak traffic:" << endl;
            hourlyPeakTraffic(trafficFlowP, outStreamP);
            outStreamP << endl;

            cout << "Report saved." << endl;
            validChoice = true;
            break;
        case 'v':
        case 'V':
            //Display the header for the report
            outStreamP << "----------------------------------------------------------------" << endl;
            outStreamP << "Peak Traffic Times Report" << endl;
            outStreamP << "----------------------------------------------------------------" << endl << endl;

            //Determine which day had the highest traffic volume
            getDayWithHighestTrafficVolume(trafficFlowP, outStreamP);

            cout << "Report saved." << endl;
            validChoice = true;
            break;
        default:
            cout << choice << " is not a valid option. Please try again and select a valid option." << endl;
            cout << "Full Report (F)" << endl << "Overall Peak Time (O)" << endl << "Peak Traffic By Day (D)" << endl << "Peak Traffic By Hour (H)" << endl << "Day With The Highest Traffic Volume (V)" << endl;
            break;
        }

        if(validChoice) //End the loop if the user selected a valid option
        {
            break;
        }
    }

    outStreamP << endl << "----------------------------------------------------------------" << endl;
}


//Congestion Level Report
/*I chose the Congestion Level Report because it can identify how regularly the road is congested, congestion levels
at certain times as well as congestion levels on certain days. Congestion levels are a great indicator of whether the road can
handle the volume of traffic and whether it needs expansion.*/

//Function to determine the values of the Congestion Level Categories
void congestionLevelCategories(deque<Vehicle> &trafficFlowP, int &lowP, int &moderateP, int &highP)
{
    if (trafficFlowP.empty())
    {
        lowP = moderateP = highP = 0; //Handle the case where the deque is empty
        return;
    }

    //Count the number of vehicles for each day
    vector<int> days(6, 0); ////We will use a vector to keep track of the number of vehicles for each day. The elements represent the days

    for (int i = 0; i < static_cast<int>(trafficFlowP.size()); i++) //The for loop iterates through the deque so that we can compare values
    {
        string timestamp = trafficFlowP[i].timeStamp;
        int day = std::stoi(timestamp.substr(8, 2)); // Get the day from the timestamp

        if (day >= 1 && day <= 6) //Ensure that the day is within valid range
        {
            days[day - 1]++; //Increase the vehicle count for the corresponding day
        }
    }

    //Get the average number of vehicles
    int sum = 0; //We need to find the total number of vehicles in order to get the average
    for (int count : days) //The loop is used to add the total number of vehicles together
    {
        sum += count;
    }
    int average = sum / 6; // Average number of vehicles per day

    //Calculate the differences between the average and the number of vehicles for each day, as well as the squares of these differences
    vector <int> differences(6);
    for (int i = 0; i < static_cast<int>(days.size()); i++) {
        differences[i] = average - days[i];
        differences[i] *= differences[i];
    }

    //Get the average of the squares
    int sumSquares = 0; //We need to get the total of the squares in order to get the average
    for (int square : differences)
    {
        sumSquares += square; //The loop is used to get the sum of the squares for each day
    }
    int averageSquares = sumSquares / 6;

    //Calculate the standard deviation
    int deviant = sqrt(averageSquares);

    //Use the standard devation to set the congestion level thresholds
    //Definitions of categories:
    //Low = average - deviant
    //If the number of vehicles is less than or equal to Low, the category is Low

    //Moderate = average + deviant
    //If the number of vehicles is greater than Low and less than or equal to Moderate, the category is Moderate

    //High
    //If the number of vehicles is greater than Moderate, the category is High
    lowP = average - deviant;
    moderateP = average + deviant;
    highP = moderateP + deviant;
}

void averageCongestionLevel(deque <Vehicle> &trafficFlowP, ostream &outStreamP)
{
    int low, moderate, high;
    congestionLevelCategories(trafficFlowP, low, moderate, high); //Determine the congestion levels

    int average = (trafficFlowP.size()) / 6; //Calculate the average congestion level by dividing the number of vehicles by the number of days
    string congestionLevel;

    if (average <= low ) //If the average is lower or equal to the variable low, then the congestion level is Low
    {
        congestionLevel = "Low";
    }
    else if (average <= moderate) //If the average is lower or equal to the variable moderate, then the congestion level is Moderate
        {
            congestionLevel = "Moderate";
        }
        else //If the congestion level is not Low or Moderate, it must be High
            {
            congestionLevel = "High";
            }

    //Display the result
    outStreamP << "Average congestion level: " << congestionLevel << endl;
}


//Function to get the most congested period
void mostCongestedPeriod(deque <Vehicle> & trafficFlowP, ostream & outStreamP)
{
    int lastDayChecked = 0; //Variable to keep track of the position of the last element before moving on to the next day

    //Variables to keep track of the date and time with the highest number of vehicles
    int dayWithHighestNumberOfVehicles, startHourHighestNumberVehicles, endHourHighestNumberVehicles;

    int overallHighestNumberVehicles = 0; //Variable to keep track of the overall highest number of vehicles
    for (int dayNumber = 1; dayNumber <= 6; ++dayNumber) //We will use a for loop to display the results for 6 days
    {
        vector <string> dayData; //Use a vector to keep track of all the data for the day
        vector <int> hours(24); //We will use a vector to keep track of the hours in each day

        for (int i = lastDayChecked; i < static_cast<int>(trafficFlowP.size()); i++) //The for loop iterates through the deque so that we can compare values
        {
            string d = trafficFlowP[i].timeStamp.substr(8, 2); //Retrieve the day from the string
            int day = std::stoi(d); //Convert the day to an integer so that it can be compared with other days

            if (i > 0 && i != lastDayChecked) //If this is not the first element, we can check to see if this is still the same day
            {
                string previousD = trafficFlowP[i - 1].timeStamp.substr(8, 2); //Retrieve the day from the previous string
                int previousDay = stoi(previousD); //Convert the day to an integer so that it can be compared with other days
                if (day == previousDay) //If this is still the same day, we can add this element to the vector for the day
                {
                    dayData.push_back(trafficFlowP[i].timeStamp); //We only need to add the time stamp to the vector, so that we can check the hours
                }
                else
                    {
                        lastDayChecked = i; //Set the variable to the last element that was checked so that we know from where to continue with the next day
                        break; //If this is not the same day anymore, the loop is ended
                    }
            }
            else
                {
                    dayData.push_back(trafficFlowP[i].timeStamp); //We need to ensure that the first element is also processed
                }

        }

        //Count the number of vehicles for each hour of the day
        for (int i = 0; i < static_cast<int>(dayData.size()); i++) //Use a loop to iterate through the elements of the vector
        {
            string hour = dayData[i].substr(11,2); //Retrieve the hour from the string
            int hr = std::stoi(hour); //Convert the hour to an integer so that it can be compared with the required hours
            hours[hr]++; //The corresponding element in the vector is updated.
        }

        //Get the hour with the highest number of vehicles
        int highest = 0; //Variable to store the highest number of vehicles
        int startHour, endHour; //Variables to store the hour which had the highest number of vehicles
        for (int i = 0; i < 24; i++)
        {
            if (hours[i] > highest) //Check whether the current element is higher than the highest number of vehicles
            {
                highest = hours[i]; //If the current element is higher than the highest number of vehicles, make that element the new highest number
                startHour = i; //Set the hour which had the highest number of vehicles to the hour represented by the current element
                if(startHour == 23) //Ensure that the hour "23:00 - 00:00" is output in the correct format
                {
                    endHour = 0; //Set the endHour variable to 00 instead of 24
                }
                else
                    {
                        endHour = startHour+1;
                    }
            }
        }

        if(highest > overallHighestNumberVehicles) /*If the highest number of vehicles is higher than the current highest number of vehicles,
        we can set the values for the date and time (as well as the number of vehicles) to the values for the current date and time*/
        {
            overallHighestNumberVehicles = highest;
            dayWithHighestNumberOfVehicles = dayNumber;
            startHourHighestNumberVehicles = startHour;
            endHourHighestNumberVehicles = endHour;
        }
    }

    //Display the result
    outStreamP << "Most congested period: Day " << dayWithHighestNumberOfVehicles << ", " << setw(2) << setfill('0') << startHourHighestNumberVehicles << ":00 - ";
    outStreamP << endHourHighestNumberVehicles << ":00, with " << overallHighestNumberVehicles << " vehicles" << endl;
}

//Function to get congestion by day
void congestionByDay(deque <Vehicle> & trafficFlowP, ostream & outStreamP)
{
    int low, moderate, high;
    congestionLevelCategories(trafficFlowP, low, moderate, high); //Determine the congestion levels
    int lastDayChecked = 0; //Variable to keep track of the position of the last element before moving on to the next day

    for (int dayNumber = 1; dayNumber <= 6; ++dayNumber) //We will use a for loop to display the results for 6 days
    {
        vector <string> dayData; //Use a vector to keep track of all the data for the day
        vector <int> hours(24); //We will use a vector to keep track of the hours in each day

        for (int i = lastDayChecked; i < static_cast<int>(trafficFlowP.size()); i++) //The for loop iterates through the deque so that we can compare values
        {
            string d = trafficFlowP[i].timeStamp.substr(8, 2); //Retrieve the day from the string
            int day = std::stoi(d); //Convert the day to an integer so that it can be compared with other days

            if (i > 0 && i != lastDayChecked) //If this is not the first element, we can check to see if this is still the same day
            {
                string previousD = trafficFlowP[i - 1].timeStamp.substr(8, 2); //Retrieve the day from the previous string
                int previousDay = stoi(previousD); //Convert the day to an integer so that it can be compared with other days
                if (day == previousDay) //If this is still the same day, we can add this element to the vector for the day
                {
                    dayData.push_back(trafficFlowP[i].timeStamp); //We only need to add the time stamp to the vector, so that we can check the hours
                }
                else
                    {
                        lastDayChecked = i; //Set the variable to the last element that was checked so that we know from where to continue with the next day
                        break; //If this is not the same day anymore, the loop is ended
                    }
            }
            else
                {
                    dayData.push_back(trafficFlowP[i].timeStamp); //We need to ensure that the first element is also processed
                }
        }

    //Count the number of vehicles for each hour of the day
    for (int i = 0; i < static_cast<int>(dayData.size()); i++) //Use a loop to iterate through the elements of the vector
    {
        string hour = dayData[i].substr(11,2); //Retrieve the hour from the string
        int hr = std::stoi(hour); //Convert the hour to an integer so that it can be compared with the required hours
        hours[hr]++; //The corresponding element in the vector is updated.
    }

    //Get the hour with the highest number of vehicles
    int highest = 0; //Variable to store the highest number of vehicles
    for (int i = 0; i < 24; i++)
    {
        if (hours[i] > highest) //Check whether the current element is higher than the highest number of vehicles
        {
            highest = hours[i]; //If the current element is higher than the highest number of vehicles, make that element the new highest number
        }
    }

    string congestionLevel;
    if(highest <= low) //If the highest number of vehicles is lower or equal to the variable low, then the congestion level is Low
    {
        congestionLevel = "Low";
    }
    else if(highest <= moderate) //If the highest number of vehicle is lower or equal to the variable moderate, then the congestion level is Moderate
        {
            congestionLevel = "Moderate";
        }
        else //If the congestion level is not Low or Moderate, it must be High
            {
                congestionLevel = "High";
            }

    // Display the result
    outStreamP << "Day " << dayNumber << ": " << congestionLevel << " (" << highest << " vehicles)" << endl;
    }
}

//Funtion to get congestion by hour
void congestionByHour(deque <Vehicle> & trafficFlowP, ostream & outStreamP)
{
   int low, moderate, high;
   congestionLevelCategories(trafficFlowP, low, moderate, high); //Determine the congestion levels
   for (int hourNumber = 0; hourNumber < 24; ++hourNumber) //We will use a for loop to display the results for every hour of the day
   {
       vector<int> days(7, 0); ////We will use a vector to keep track of the total vehicles for each day
       for (int i = 0; i < static_cast<int>(trafficFlowP.size()); i++)
       {
           string hr = trafficFlowP[i].timeStamp.substr(11, 2); //Retrieve the hour from the string
           string d = trafficFlowP[i].timeStamp.substr(8, 2);

           int hour = std::stoi(hr); //Convert the hour to an integer so that it can be compared with other hours
           int day = std::stoi(d);

           //If this is the specified hour, we can update the count for the corresponding day
           if (hour == hourNumber)
           {
               days[day - 1]++;
           }
        }

        //Get the day with the highest number of vehicles
        int highest = 0; //Variable to store the highest number of vehicles
        for (int j = 0; j < static_cast<int>(days.size()); j++)
        {
            if (days[j] > highest) //Check whether the current element is higher than the highest number of vehicles
            {
                highest = days[j]; //If the current element is higher than the highest number of vehicles, make that element the new highest number
            }
        }

        string congestionLevel;
        if (highest <= low) //If the highest number of vehicles is lower or equal to the variable low, then the congestion level is Low
        {
            congestionLevel = "Low";
        }
        else if (highest <= moderate) //If the highest number of vehicle is lower or equal to the variable moderate, then the congestion level is Moderate
            {
                congestionLevel = "Moderate";
            }
            else //If the congestion level is not Low or Moderate, it must be High
                {
                    congestionLevel = "High";
                }

        // Display the result
        int endHour;
        if(hourNumber == 23) //Ensure that the hour "23:00 - 00:00" is output in the correct format
        {
            endHour = 0; //Set the endHour variable to 00 instead of 24
        }
        else
            {
                endHour=hourNumber+1;
            }

        outStreamP << setw(2) << setfill('0') << hourNumber << ":00 - " << setw(2) << setfill('0') << endHour << ":00 : " << congestionLevel << "\t(" << highest << " vehicles)" << endl;
    }
}

//Function for Congestion Level Report
void congestionLevelReport(deque <Vehicle> trafficFlowP, ostream & outStreamP)
{
    char choice;
    bool validChoice = false; //Variable to check whether a valid option was selected
    cout << "Please select the part of the report to generate: " << endl;
    cout << "Full Report (F)" << endl << "Average Congestion Level (A)" << endl << "Most Congested Period (M)" << endl << "Congestion By Day (D)" << endl << "Congestion By Hour (H)" << endl;

    while(cin >> choice)
    {
        //I am using a switch to allow customization for generating the report
        switch(choice)
        {
        case 'f':
        case 'F':
            //Display the header for the report
            outStreamP << "----------------------------------------------------------------" << endl;
            outStreamP << "Congestion Level Report" << endl;
            outStreamP << "----------------------------------------------------------------" << endl << endl;

            //Display the average congestion level
            averageCongestionLevel(trafficFlowP, outStreamP);
            outStreamP << endl;

            //Display the most congested period
            mostCongestedPeriod(trafficFlowP, outStreamP);
            outStreamP << endl;

            //Display congestion by day
            outStreamP << "Congestion breakdown by day:" << endl;
            congestionByDay(trafficFlowP, outStreamP);
            outStreamP << endl;

            //Display congestion by hour
            outStreamP << "Congestion breakdown by hour:" << endl;
            congestionByHour(trafficFlowP, outStreamP);

            cout << "Report saved." << endl;
            validChoice = true;
            break;
        case 'a':
        case 'A':
            //Display the header for the report
            outStreamP << "----------------------------------------------------------------" << endl;
            outStreamP << "Congestion Level Report" << endl;
            outStreamP << "----------------------------------------------------------------" << endl << endl;

            //Display the average congestion level
            averageCongestionLevel(trafficFlowP, outStreamP);
            outStreamP << endl;

            cout << "Report saved." << endl;
            validChoice = true;
            break;
        case 'm':
        case 'M':
            //Display the header for the report
            outStreamP << "----------------------------------------------------------------" << endl;
            outStreamP << "Congestion Level Report" << endl;
            outStreamP << "----------------------------------------------------------------" << endl << endl;

            //Display the most congested period
            mostCongestedPeriod(trafficFlowP, outStreamP);
            outStreamP << endl;

            cout << "Report saved." << endl;
            validChoice = true;
            break;
        case 'd':
        case 'D':
            //Display the header for the report
            outStreamP << "----------------------------------------------------------------" << endl;
            outStreamP << "Congestion Level Report" << endl;
            outStreamP << "----------------------------------------------------------------" << endl << endl;

            //Display congestion by day
            outStreamP << "Congestion breakdown by day:" << endl;
            congestionByDay(trafficFlowP, outStreamP);
            outStreamP << endl;

            cout << "Report saved." << endl;
            validChoice = true;
            break;
        case 'h':
        case 'H':
            //Display the header for the report
            outStreamP << "----------------------------------------------------------------" << endl;
            outStreamP << "Congestion Level Report" << endl;
            outStreamP << "----------------------------------------------------------------" << endl << endl;

            //Display congestion by hour
            outStreamP << "Congestion breakdown by hour:" << endl;
            congestionByHour(trafficFlowP, outStreamP);

            cout << "Report saved." << endl;
            validChoice = true;
            break;
        default:
            cout << choice << " is not a valid option. Please try again and select a valid option." << endl;
            cout << "Full Report (F)" << endl << "Average Congestion Level (A)" << endl << "Most Congested Period (M)" << endl << "Congestion By Day (D)" << endl << "Congestion By Hour (H)" << endl;
        }

         if(validChoice) //End the loop if the user selected a valid option
        {
            break;
        }
    }

    outStreamP << endl << "----------------------------------------------------------------" << endl;
}


//Vehicle-Type Specific Report
/*I chose to do a Vehicle-Type Specific Report because the type of vehicle can also have an effect on traffic flow and congestion.
For example, 10 motorcycles will pass through a road more quickly than 10 trucks.
*/

//Function to get the total number of cars
void totalCars(deque <Vehicle> & trafficFlowP, ostream & outStreamP)
{
    int totalNumberOfCars = 0; //Variable to store the total number of specified vehicles
    for(int i = 0; i < static_cast<int>(trafficFlowP.size()); i++) //Use a for loop to iterate through the deque so that we can compare values
    {
        if(trafficFlowP[i].vehicleType == "CAR") //Check if the vehicle type in the deque is the same as the specified vehicle type
            {
                totalNumberOfCars++; //Increase the vehicle count if the vehicle types match

            }
    }

    //Display the result
    outStreamP << "Total cars: " << totalNumberOfCars << endl;
}

//Function to get the peak time for cars
void peakTimeForCars(deque <Vehicle> & trafficFlowP, ostream& outStreamP)
{
    //I used a vector to keep track of the number of specified vehicles per hour. Each element represents an hour.
    vector <int> hours(24);
    for (int i = 0; i < static_cast<int>(trafficFlowP.size()); i++) //Use a loop to iterate through the elements of the deque
    {
        if(trafficFlowP[i].vehicleType == "CAR") //If the current vehicle type matches the specified vehicle type, the vectors are updated
        {
            string hour = trafficFlowP[i].timeStamp.substr(11,2); //Retrieve the hour from the string
            int hr = std::stoi(hour); //Convert the hour to an integer so that it can be compared with the required hours
            hours[hr]++; //The corresponding element in the vector is updated.
        }
    }

    //Get the hour with the highest total number of specified vehicles
    int highest = 0; //Variable to store the highest number of vehicles
    int startHour, endHour; //Variables to store the hour which had the highest number of vehicles
    for(int i = 0; i < 24; i++)
    {
        if(hours[i] > highest) //Check whether the current element is higher than the highest number of vehicles
        {
            highest = hours[i]; //If the current element is higher than the highest number of vehicles, make that element the new highest number
            startHour = i; //Set the hour which had the highest number of vehicles to the hour represented by the current element
            if(startHour == 23) //Ensure that the hour "23:00 - 00:00" is output in the correct format
            {
                endHour=0; //Set the endHour variable to 00 instead of 24
            }
            else
                {
                    endHour = startHour + 1;
                }
        }
    }

    //Output the overall peak traffic time
    outStreamP << "Peak time: " << setw(2) << setfill('0') << startHour << ":00 - " << setw(2) << setfill('0') << endHour << ":00, " << highest << " Cars" << endl;
}

//Function to get the average speed for cars
void averageSpeedCars(deque <Vehicle> & trafficFlowP, ostream & outStreamP)
{
    int totalVehicles = 0; //Variable for the total number of specified vehicles
    int sum = 0; //Variable for total speeds of the specified vehicle
    float average; //Variable for the average speed of the specified vehicle

    for (int i = 0; i < static_cast<int>(trafficFlowP.size()); i++) //Use a for loop to iterate through the deque so we can compare vehicle types
    {
        if(trafficFlowP[i].vehicleType == "CAR")
        {
            totalVehicles++; //Increase the vehicle count
            sum += trafficFlowP[i].speed; //Update the total speeds of the specified vehicle
        }
    }
    average = float(sum) / totalVehicles; /*Calculate the average speed. Explicitly converting sum to a float
    will ensure a more accurate porrayal of the average speed*/

    //Set the flags so that the speed will output to 2 decimal places
    outStreamP.setf(ios::fixed);
    outStreamP.precision(2);
    //Display the result
    outStreamP << "Average speed: " << average << " km/h" << endl;
}

//Function for Vehicle-Type Specific Report - Cars
void vehicleTypeReportCars(deque <Vehicle> & trafficFlowP, ostream& outStreamP)
{
    //Display the header for the report
    outStreamP << "----------------------------------------------------------------" << endl;
    outStreamP << "Vehicle Type Report (Cars):" << endl;
    outStreamP << "----------------------------------------------------------------" << endl << endl;

    //Display total vehicles of the type specified
    totalCars(trafficFlowP, outStreamP);

    //Display peak time for the vehicle type
    peakTimeForCars(trafficFlowP, outStreamP);

    //Display the average speed for the vehicle type
    averageSpeedCars(trafficFlowP, outStreamP);
    outStreamP << endl << "----------------------------------------------------------------" << endl;
}

//Function to get the total number of buses
void totalBuses(deque <Vehicle> & trafficFlowP, ostream& outStreamP)
{
    int totalNumberOfBuses = 0; //Variable to store the total number of specified vehicles
    for(int i = 0; i < static_cast<int>(trafficFlowP.size()); i++) //Use a for loop to iterate through the deque so that we can compare values
    {
        if(trafficFlowP[i].vehicleType == "BUS") //Check if the vehicle type in the deque is the same as the specified vehicle type
            {
                totalNumberOfBuses++; //Increase the vehicle count if the vehicle types match

            }
    }

    //Display the result
    outStreamP << "Total buses: " << totalNumberOfBuses << endl;
}

//Function to get the peak time for buses
void peakTimeForBuses(deque <Vehicle> & trafficFlowP, ostream& outStreamP)
{
    //I used a vector to keep track of the number of specified vehicles per hour. Each element represents an hour.
    vector <int> hours(24);
    for (int i = 0; i < static_cast<int>(trafficFlowP.size()); i++) //Use a loop to iterate through the elements of the deque
    {
        if(trafficFlowP[i].vehicleType == "BUS") //If the current vehicle type matches the specified vehicle type, the vectors are updated
        {
            string hour = trafficFlowP[i].timeStamp.substr(11,2); //Retrieve the hour from the string
            int hr = std::stoi(hour); //Convert the hour to an integer so that it can be compared with the required hours
            hours[hr]++; //The corresponding element in the vector is updated.
        }
    }

    //Get the hour with the highest total number of specified vehicles
    int highest = 0; //Variable to store the highest number of vehicles
    int startHour, endHour; //Variables to store the hour which had the highest number of vehicles
    for(int i = 0; i < 24; i++)
    {
        if(hours[i] > highest) //Check whether the current element is higher than the highest number of vehicles
        {
            highest = hours[i]; //If the current element is higher than the highest number of vehicles, make that element the new highest number
            startHour = i; //Set the hour which had the highest number of vehicles to the hour represented by the current element
            if(startHour == 23) //Ensure that the hour "23:00 - 00:00" is output in the correct format
            {
                endHour = 0; //Set the endHour variable to 00 instead of 24
            }
            else
                {
                    endHour = startHour + 1;
                }
        }
    }

    //Output the overall peak traffic time
    outStreamP << "Peak time: " << setw(2) << setfill('0') << startHour << ":00 - " << setw(2) << setfill('0') << endHour << ":00, " << highest << " Buses" << endl;
}

//Function to get the average speed for buses
void averageSpeedBuses(deque <Vehicle> & trafficFlowP, ostream& outStreamP)
{
    int totalVehicles = 0; //Variable for the total number of specified vehicles
    int sum = 0; //Variable for total speeds of the specified vehicle
    float average; //Variable for the average speed of the specified vehicle

    for (int i = 0; i < static_cast<int>(trafficFlowP.size()); i++) //Use a for loop to iterate through the deque so we can compare vehicle types
    {
        if(trafficFlowP[i].vehicleType == "BUS")
        {
            totalVehicles++; //Increase the vehicle count
            sum += trafficFlowP[i].speed; //Update the total speeds of the specified vehicle
        }
    }
    average = float(sum) / totalVehicles; /*Calculate the average speed. Explicitly converting sum to a float
    will ensure a more accurate porrayal of the average speed*/

    //Set the flags so that the speed will output to 2 decimal places
    outStreamP.setf(ios::fixed);
    outStreamP.precision(2);
    //Display the result
    outStreamP << "Average speed: " << average << " km/h" << endl;
}

//Function for Vehicle-Type Specific Report - Buses
void vehicleTypeReportBuses(deque <Vehicle> & trafficFlowP, ostream& outStreamP)
{
    //Display the header for the report
    outStreamP << "----------------------------------------------------------------" << endl;
    outStreamP << "Vehicle Type Report (Buses):" << endl;
    outStreamP << "----------------------------------------------------------------" << endl << endl;

    //Display total vehicles of the type specified
    totalBuses(trafficFlowP, outStreamP);

    //Display peak time for the vehicle type
    peakTimeForBuses(trafficFlowP, outStreamP);

    //Display the average speed for the vehicle type
    averageSpeedBuses(trafficFlowP, outStreamP);
    outStreamP << endl << "----------------------------------------------------------------" << endl;
}

//Function to get the total number of motorcycles
void totalMotorcycles(deque <Vehicle> & trafficFlowP, ostream& outStreamP)
{
    int totalNumberOfMotorcycles = 0; //Variable to store the total number of specified vehicles
    for(int i = 0; i < static_cast<int>(trafficFlowP.size()); i++) //Use a for loop to iterate through the deque so that we can compare values
    {
        if(trafficFlowP[i].vehicleType == "MOTORCYCLE") //Check if the vehicle type in the deque is the same as the specified vehicle type
            {
                totalNumberOfMotorcycles++; //Increase the vehicle count if the vehicle types match

            }
    }

    //Display the result
    outStreamP << "Total motorcycles: " << totalNumberOfMotorcycles<<endl;
}

//Function to get the peak time for motorcycles
void peakTimeForMotorcycles(deque <Vehicle> & trafficFlowP, ostream& outStreamP)
{
    //I used a vector to keep track of the number of specified vehicles per hour. Each element represents an hour.
    vector<int> hours(24);
    for (int i = 0; i < static_cast<int>(trafficFlowP.size()); i++) //Use a loop to iterate through the elements of the deque
    {
        if(trafficFlowP[i].vehicleType == "MOTORCYCLE") //If the current vehicle type matches the specified vehicle type, the vectors are updated
        {
            string hour = trafficFlowP[i].timeStamp.substr(11,2); //Retrieve the hour from the string
            int hr = std::stoi(hour); //Convert the hour to an integer so that it can be compared with the required hours
            hours[hr]++; //The corresponding element in the array is updated.
        }
    }

    //Get the hour with the highest total number of specified vehicles
    int highest = 0; //Variable to store the highest number of vehicles
    int startHour, endHour; //Variables to store the hour which had the highest number of vehicles
    for(int i = 0; i < 24; i++)
    {
        if(hours[i] > highest) //Check whether the current element is higher than the highest number of vehicles
        {
            highest = hours[i]; //If the current element is higher than the highest number of vehicles, make that element the new highest number
            startHour = i; //Set the hour which had the highest number of vehicles to the hour represented by the current element
            if(startHour == 23) //Ensure that the hour "23:00 - 00:00" is output in the correct format
            {
                endHour = 0; //Set the endHour variable to 00 instead of 24
            }
            else
                {
                    endHour = startHour + 1;
                }
        }
    }

    //Output the overall peak traffic time
    outStreamP << "Peak time: " << setw(2) << setfill('0') << startHour << ":00 - " << setw(2) << setfill('0') << endHour << ":00, " << highest << " Motorcycles" << endl;
}

//Function to get the average speed for motorcycles
void averageSpeedMotorcycles(deque <Vehicle> & trafficFlowP, ostream& outStreamP)
{
    int totalVehicles = 0; //Variable for the total number of specified vehicles
    int sum = 0; //Variable for total speeds of the specified vehicle
    float average; //Variable for the average speed of the specified vehicle

    for (int i = 0; i < static_cast<int>(trafficFlowP.size()); i++) //Use a for loop to iterate through the deque so we can compare vehicle types
    {
        if(trafficFlowP[i].vehicleType == "MOTORCYCLE")
        {
            totalVehicles++; //Increase the vehicle count
            sum += trafficFlowP[i].speed; //Update the total speeds of the specified vehicle
        }
    }
    average = float(sum) / totalVehicles; /*Calculate the average speed. Explicitly converting sum to a float
    will ensure a more accurate porrayal of the average speed*/

    //Set the flags so that the speed will output to 2 decimal places
    outStreamP.setf(ios::fixed);
    outStreamP.precision(2);
    //Display the result
    outStreamP << "Average speed: " << average << " km/h" << endl;
}

//Function for Vehicle-Type Specific Report - Motorcycles
void vehicleTypeReportMotorcycles(deque <Vehicle> & trafficFlowP, ostream& outStreamP)
{
    //Display the header for the report
    outStreamP << "----------------------------------------------------------------" << endl;
    outStreamP << "Vehicle Type Report (Motorcycles):" << endl;
    outStreamP << "----------------------------------------------------------------" << endl << endl;

    //Display total vehicles of the type specified
    totalMotorcycles(trafficFlowP, outStreamP);

    //Display peak time for the vehicle type
    peakTimeForMotorcycles(trafficFlowP, outStreamP);

    //Display the average speed for the vehicle type
    averageSpeedMotorcycles(trafficFlowP, outStreamP);
    outStreamP << endl << "----------------------------------------------------------------" << endl;
}

//Function to get the total number of trucks
void totalTrucks(deque <Vehicle> & trafficFlowP, ostream& outStreamP)
{
    int totalNumberOfTrucks = 0; //Variable to store the total number of specified vehicles
    for(int i = 0; i < static_cast<int>(trafficFlowP.size()); i++) //Use a for loop to iterate through the deque so that we can compare values
    {
        if(trafficFlowP[i].vehicleType == "TRUCK") //Check if the vehicle type in the deque is the same as the specified vehicle type
            {
                totalNumberOfTrucks++; //Increase the vehicle count if the vehicle types match

            }
    }

    //Display the result
    outStreamP << "Total trucks: " << totalNumberOfTrucks<<endl;
}

//Function to get the peak time for trucks
void peakTimeForTrucks(deque <Vehicle> & trafficFlowP, ostream& outStreamP)
{
    //I used a vector to keep track of the number of specified vehicles per hour. Each element represents an hour.
    vector <int> hours(24);
    for (int i = 0; i < static_cast<int>(trafficFlowP.size()); i++) //Use a loop to iterate through the elements of the deque
    {
        if(trafficFlowP[i].vehicleType == "TRUCK") //If the current vehicle type matches the specified vehicle type, the vectors are updated
        {
            string hour = trafficFlowP[i].timeStamp.substr(11,2); //Retrieve the hour from the string
            int hr = std::stoi(hour); //Convert the hour to an integer so that it can be compared with the required hours
            hours[hr]++; //The corresponding element in the array is updated.
        }
    }

    //Get the hour with the highest total number of specified vehicles
    int highest = 0; //Variable to store the highest number of vehicles
    int startHour, endHour; //Variables to store the hour which had the highest number of vehicles
    for(int i = 0; i < 24; i++)
    {
        if(hours[i] > highest) //Check whether the current element is higher than the highest number of vehicles
        {
            highest = hours[i]; //If the current element is higher than the highest number of vehicles, make that element the new highest number
            startHour = i; //Set the hour which had the highest number of vehicles to the hour represented by the current element
            if(startHour == 23) //Ensure that the hour "23:00 - 00:00" is output in the correct format
            {
                endHour = 0; //Set the endHour variable to 00 instead of 24
            }
            else
                {
                    endHour = startHour + 1;
                }
        }
    }

    //Output the overall peak traffic time
    outStreamP << "Peak time: " << setw(2) << setfill('0') << startHour << ":00 - " << setw(2) << setfill('0') << endHour << ":00, " << highest << " Trucks" << endl;
}

//Function to get the average speed for trucks
void averageSpeedTrucks(deque <Vehicle> & trafficFlowP, ostream& outStreamP)
{
    int totalVehicles = 0; //Variable for the total number of specified vehicles
    int sum = 0; //Variable for total speeds of the specified vehicle
    float average; //Variable for the average speed of the specified vehicle

    for (int i = 0; i < static_cast<int>(trafficFlowP.size()); i++) //Use a for loop to iterate through the deque so we can compare vehicle types
    {
        if(trafficFlowP[i].vehicleType == "TRUCK")
        {
            totalVehicles++; //Increase the vehicle count
            sum += trafficFlowP[i].speed; //Update the total speeds of the specified vehicle
        }
    }
    average = float(sum) / totalVehicles; /*Calculate the average speed. Explicitly converting sum to a float
    will ensure a more accurate potrayal of the average speed*/ average = sum / totalVehicles; //Calculate the average speed

    //Set the flags so that the speed will output to 2 decimal places
    outStreamP.setf(ios::fixed);
    outStreamP.precision(2);
    //Display the result
    outStreamP << "Average speed: " << average << " km/h" << endl;
}

//Function for Vehicle-Type Specific Report - Motorcycles
void vehicleTypeReportTrucks(deque <Vehicle> & trafficFlowP, ostream& outStreamP)
{
    //Display the header for the report
    outStreamP << "----------------------------------------------------------------" << endl;
    outStreamP << "Vehicle Type Report (Trucks):" << endl;
    outStreamP << "----------------------------------------------------------------" << endl << endl;

    //Display total vehicles of the type specified
    totalTrucks(trafficFlowP, outStreamP);

    //Display peak time for the vehicle type
    peakTimeForTrucks(trafficFlowP, outStreamP);

    //Display the average speed for the vehicle type
    averageSpeedTrucks(trafficFlowP, outStreamP);
    outStreamP << endl << "----------------------------------------------------------------" << endl;
}

//Function for Vehicle-Type Specific Report
void vehicleTypeSpecificReport(deque <Vehicle> & trafficFlowP, ostream& outStreamP)
//This function uses a switch to allow the user to select the vehicle type for the report
{
    char choice; //Variable to store the option selected by the user
    bool validChoice = false; //Variable to check whether a valid option was selected
    cout << "Please select the vehicle type for which you want to generate a report:" << endl;
    cout << "Buses (B)" << endl << "Cars (C)" << endl << "Motorcycles (M)" << endl << "Trucks (T)" <<endl;

    while(cin >> choice)
    {
        switch(choice)
        {
        case 'b':
        case 'B':
            vehicleTypeReportBuses(trafficFlowP, outStreamP);
            validChoice = true;
            cout << "Report saved." << endl;
            break;
        case 'c':
        case 'C':
            vehicleTypeReportCars(trafficFlowP, outStreamP);
            validChoice = true;
            cout << "Report saved." << endl;
            break;
        case 'm':
        case 'M':
            vehicleTypeReportMotorcycles(trafficFlowP, outStreamP);
            validChoice = true;
            cout << "Report saved." << endl;
            break;
        case 't':
        case 'T':
            vehicleTypeReportTrucks(trafficFlowP, outStreamP);
            validChoice = true;
            cout << "Report saved." << endl;
            break;
        default:
            cout << choice << " is not a valid option. Please try again and select a valid option." << endl;
            cout << "Buses (B)" << endl << "Cars (C)" << endl << "Motorcycles (M)" << endl << "Trucks (T)" << endl;
        }

        if(validChoice) //End the loop if the user selected a valid option
        {
            break;
        }
    }
}

int main()
{
    deque <Vehicle> trafficFlow; //The deque will be used to process the information about the traffic flow
    string vehicleData; //This string will be used to read input from the text file

    //Reading data from the text file
    string filepath = "trafficdata.txt"; //The string variable specifies the path for the text file
    ifstream inStream; //Creates an input stream for reading data from the file
    inStream.open(filepath); //Open the file
    if(inStream.fail()) //Make sure that the file has opened correctly
    {
        cout << "Opening input file failed." << endl; //If the file has not opened correctly, the program displays an error message
        exit(1);
    }

    //Process data from the text file
    getline(inStream, vehicleData); //Ignore the first line
    while (getline(inStream, vehicleData))
    {
        if (vehicleData.empty()) //Check wheher the line is empty
        {
            continue; //Skip the line
        }

        //Find the positions of the delimiters
        int firstDelimeter = vehicleData.find(';');
        int secondDelimeter = vehicleData.find(';', firstDelimeter + 1);

        //Use string size to check for valid indices
        if (firstDelimeter >= static_cast<int>(vehicleData.size()) || secondDelimeter >= static_cast<int>(vehicleData.size()))
        {
            continue; //Skip malformed lines
        }

        //Extract the timestamp, vehicle type, and speed from the strings
        string tStamp = vehicleData.substr(0, firstDelimeter);
        string vType = vehicleData.substr(firstDelimeter + 1, secondDelimeter - firstDelimeter - 1);
        string speedStr = vehicleData.substr(secondDelimeter + 1);

        //Ensure that speedStr is not empty
        if (speedStr.empty())
        {
            continue; //Skip if speed is missing
        }

        int vehicleSpeed = std::stoi(speedStr); //Convert the speed to an integer

        //Remove spaces from vehicle type, if there are any
        string v;
        for (char ch : vType)
        {
            if (!isspace(ch))
            {
                v += ch; //Append non-space characters to the string
            }
        }
        vType = v; //Update the original vehicle type string

        Vehicle currentVehicle{tStamp, vType, vehicleSpeed}; //Initialize the member variables of the current vehicle
        trafficFlow.push_back(currentVehicle); //Add the current vehicle to the deque
    }

    inStream.close(); //Close the text file as soon as it is no longer needed
    //Writing output to the textfile
    ofstream outStream("trafficReport.txt"); //Creates an output stream and output file for writing the report

    if(outStream.fail()) //Make sure that the file has opened correctly
    {
        cout << "Opening output file failed." << endl; //If the file has not opened correctly, the program displays an error message
        exit(1);
    }

    char choice; //Variable to store the choice made by the user
    bool validChoice = false; //Variable to check whether a valid option was selected
    displayHeader(); //Displays the header

    //The cout statements display the options to the user
    cout << endl << "Choose a report to generate: " << endl << endl;
    cout << "Peak Traffic Times Report (P)" << endl << "Congestion Level Report (C)" << endl << "Vehicle-Type Specific Report (V)" << endl;

    while(cin >> choice)
    {
        //I am using a switch to generate the report corresponding to the letter the user chooses
        switch(choice)
        {
            case 'p':
            case 'P':
                peakTrafficTimesReport(trafficFlow, outStream);
                validChoice = true;
                break;
            case 'c':
            case 'C':
                congestionLevelReport(trafficFlow, outStream);
                validChoice = true;
                break;
            case 'v':
            case 'V':
                vehicleTypeSpecificReport(trafficFlow, outStream);
                validChoice = true;
                break;
            default:
                cout << choice << " is not a valid option. Please try again and select a valid option." << endl;
                cout << "Peak Traffic Times Report (P)" << endl << "Congestion Level Report (C)" << endl << "Vehicle-Type Specific Report (V)" << endl;
        }
        if(validChoice) //End the loop if the user selected a valid option
        {
            break;
        }
    }

    outStream.close(); //Close the file as soon as it is no longer needed
    return 0;
}

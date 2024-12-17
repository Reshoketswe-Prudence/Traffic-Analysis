//Name: Tlou Reshoketswe Prudence.

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>
#include <string>

using namespace std;

// Struct to store traffic data
struct TrafficData
{
    string timeStamp;
    string vehicle;
    float speed;

};

//Function to check if a string is a float
bool isValidFloat(const string& str)
{
    stringstream ss(str);
    float f;
    return (ss >> f) && (ss.eof());
}

//Function to read traffic data from traffic data file
vector<TrafficData> readTrafficData(const string& filename)
{
    vector<TrafficData> data;
    ifstream file(filename);

    if (!file.is_open())
    {
        cerr << "Error: could not open file " << filename << endl;
        return data;
    }

    string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue;  //Skips empty lines

        stringstream ss(line);
        string time, vehicleType, vehicleSpeed;

        if (!getline(ss, time, ';') || !getline(ss, vehicleType, ';') || !getline(ss, vehicleSpeed, ';'))
        {
            cerr << "Warning: Invalid line format, skipping: " << line << endl;
            continue;
        }

        //Validates if vehicleSpeed is a float
        if (isValidFloat(vehicleSpeed))
        {
            float speed = stof(vehicleSpeed);  //stof to convert string to a float
            data.push_back({time, vehicleType, speed});
        }
        else
        {
            continue;
        }
    }

    file.close();
    return data;
}

//Function to extract hours from timestamp
string gettingHours(const string& timestamp)
{
    return timestamp.substr(11, 2);
}
//Function to extract days from timestamp
string gettingDays(const string& timestamp)
{
    return timestamp.substr(0, 10);
}

bool isWithinTimeRange(const string& timestamp, const string& startHour, const string& endHour)
{
    string hour;
    hour = gettingHours(timestamp);
    return(hour >= startHour && hour < endHour);
}
//Chosen reports from the brief we were given - report 2, report 5 and report 6
//Function to generate Report 2: Peak Traffic Times Report
void generatePeakTrafficTimes(const vector<TrafficData>& data)
{
    //Maps to store the vehicle count for each hour and each day
    map<string, map<string, int>> hourlyCountByDay;
    map<string, string> dayLabelMap;  // To map dates to a particular day

    int dayCounter = 1;

    //Generates hourly traffic count for each day
    for (const auto& record : data)
    {
        string date = gettingDays(record.timeStamp);
        string hour = gettingHours(record.timeStamp);

        //If the date is not in the map, assign a new day
        if (dayLabelMap.find(date) == dayLabelMap.end())
        {
            dayLabelMap[date] = "Day " + to_string(dayCounter++);
        }

        hourlyCountByDay[date][hour]++;
    }

    string peakTime;
    int maximumVehicles = 0;

    cout << "Peak Traffic Times Report:" << endl;
    cout << "--------------------------------------------------------" << endl;
    for (const auto& day : hourlyCountByDay)
    {
        string peakHour;
        int maxVehicles = 0;

        for (const auto& hour : day.second)
        {
            if (hour.second > maxVehicles)
            {
                maxVehicles = hour.second;
                peakHour = hour.first;
            }
        }

        int startHour = stoi(peakHour);
        cout << dayLabelMap[day.first] << ": " << peakHour << ":00, " << maxVehicles << " vehicles" << endl;

        if (maxVehicles > maximumVehicles)
        {
            maximumVehicles = maxVehicles;
            peakTime = dayLabelMap[day.first] + ", " + peakHour + ":00";
        }
    }

    cout << endl;
    cout << "Peak traffic time (overall): " << peakTime << " with " << maximumVehicles << " vehicles" << endl;
}

//Function to generate Report 5: Custom Time Interval Report
void generateCustomIntervalReport(const vector<TrafficData>& data, const string& startHour, const string& endHour)
{
    int totalVehicles = 0;
    float totalSpeed = 0;
    map<string, int> trafficByHour;

    for (const auto& record : data)
    {
        if (isWithinTimeRange(record.timeStamp, startHour, endHour))
        {
            totalVehicles++;
            totalSpeed += record.speed;
            trafficByHour[gettingHours(record.timeStamp)]++;
        }
    }

    if (totalVehicles == 0)
    {
        cout << "No data found in the specified time interval (" << startHour << " to " << endHour << ")" << endl;
        return;
    }

    string mostCongestedHour;
    int maxTraffic = 0;
    for (const auto& hourData : trafficByHour)
    {
        if (hourData.second > maxTraffic)
        {
            maxTraffic = hourData.second;
            mostCongestedHour = hourData.first;
        }
    }

    cout << "Custom Interval Report (" << startHour << " to " << endHour << "):" << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << "Total vehicles: " << totalVehicles << endl;
    cout << "Average speed: " << fixed << setprecision(2) << (totalSpeed / totalVehicles) << " km/h" << endl;
    cout << "Most congested time: " << mostCongestedHour << ":00 - " << stoi(mostCongestedHour) + 1 << ":00, " << maxTraffic << " vehicles" << endl;
}

//Function to generate Report 6: Vehicle Type-Specific Report
void generateVehicleTypeReport(const vector<TrafficData>& data, const string& vehicleType)
{
    int totalVehicles = 0;
    float totalSpeed = 0;
    map<string, int> trafficByHour;

    for (const auto& record : data)
    {
        if (record.vehicle == vehicleType)
        {
            totalVehicles++;
            totalSpeed += record.speed;
            trafficByHour[gettingHours(record.timeStamp)]++;
        }
    }

    if (totalVehicles == 0)
    {
        cout << "No data found for the vehicle type: " << vehicleType << endl;
        return;
    }

    string peakHour;
    int maxTraffic = 0;
    for (const auto& hourData : trafficByHour)
    {
        if (hourData.second > maxTraffic)
        {
            maxTraffic = hourData.second;
            peakHour = hourData.first;
        }
    }

    cout << "Vehicle Type Report (" << vehicleType << "):" << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << "Total " << vehicleType << "ES: " << totalVehicles << endl;
    cout << "Peak time: " << peakHour << ":00 - " << stoi(peakHour) + 1 << ":00, " << maxTraffic << " " << vehicleType << "ES" << endl;
    cout << "Average speed: " << fixed << setprecision(2) << (totalSpeed / totalVehicles) << " km/h" << endl;
}

int main()
{
    string filename = "C:\\finalproject\\data\\trafficdata.txt";
    vector<TrafficData> data = readTrafficData(filename);

    if (data.empty())
    {
        cout << "No data available to analyze" << endl;
        return 1;
    }

    generatePeakTrafficTimes(data);
    cout << endl;
    generateCustomIntervalReport(data, "09:00", "12:00");
    cout << endl;
    generateVehicleTypeReport(data, "BUS");

    return 0;
}


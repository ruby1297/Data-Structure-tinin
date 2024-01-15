// Implement Banker’s algorithm
#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <fstream>
#include <algorithm>

using namespace std;

class BankerAlgorithm
{
public:
    BankerAlgorithm(vector<int> Available)
    {
        this->available = Available;
    }

    void InResources(string ID, vector<int> max_claim)
    {
        this->Need[ID] = max_claim;
        this->max_claim[ID] = max_claim;
        this->allocation[ID] = vector<int>(max_claim.size(), 0);
    }

    void requestResources(string ID, vector<int> request)
    {
        if (isValidRequest(ID, request))
        {
            for (int i = 0; i < request.size(); i++)
            {
                this->available[i] -= request[i];
                this->Need[ID][i] -= request[i];
                this->allocation[ID][i] += request[i];
            }
            if (!safe())
                for (int i = 0; i < request.size(); i++)
                {
                    this->available[i] += request[i];
                    this->Need[ID][i] += request[i];
                    this->allocation[ID][i] -= request[i];
                }
        }
    }

    bool isValidRequest(string ID, vector<int> request)
    {
        if (this->max_claim.find(ID) == this->max_claim.end())
            return false;

        for (int i = 0; i < request.size(); i++)
        {
            if (request[i] > this->Need[ID][i])
                return false;
            if (request[i] > this->available[i])
                return false;
            if (request[i] + this->allocation[ID][i] > this->max_claim[ID][i])
                return false;
        }

        return true;
    }

    bool safe()
    {
        vector<int> work = available;
        unordered_map<string, bool> Finish;

        for (auto &it : allocation)
            Finish[it.first] = false;

        while (true)
        {
            bool found = false;
            for (auto &it : allocation)
            {
                if (!Finish[it.first])
                {
                    bool enough = true;
                    for (int i = 0; i < it.second.size(); i++)
                        if (Need[it.first][i] > work[i])
                        {
                            enough = false;
                            break;
                        }

                    if (enough)
                    {
                        for (int i = 0; i < it.second.size(); i++)
                            work[i] += it.second[i];

                        Finish[it.first] = true;
                        found = true;
                    }
                }
            }

            if (!found)
                break;
        }

        for (auto &item : Finish)
            if (!item.second)
                return false;

        return true;
    }

    void wantoutResources(string ID)
    {
        if (isValidwantout(ID, this->Need[ID]))
        {
            for (int i = 0; i < this->allocation[ID].size(); i++)
                this->available[i] += this->allocation[ID][i];

            this->allocation.erase(ID);
            this->max_claim.erase(ID);
            this->Need.erase(ID);
        }
    }

    bool isValidwantout(const string &ID, const vector<int> &release)
    {
        if (this->allocation.find(ID) == this->allocation.end())
            return false;

        for (int i = 0; i < release.size(); i++)
            if (release[i] > this->available[i])
                return false;

        return true;
    }

    void output()
    {
        vector<string> id;
        for (auto it = this->allocation.begin(); it != this->allocation.end(); ++it)
        {
            id.push_back(it->first);
        }
        sort(id.begin(), id.end());
        for (int i = 0; i < id.size(); i++)
        {
            cout << id[i] << " ";
            for (int j = 0; j < this->allocation[id[i]].size(); j++)
            {
                cout << this->allocation[id[i]][j];
                if (j != this->allocation[id[i]].size() - 1)
                    cout << ",";
            }
            cout << endl;
        }
    }

private:
    unordered_map<string, vector<int>> allocation;
    unordered_map<string, vector<int>> max_claim;
    vector<int> available;
    unordered_map<string, vector<int>> Need;
};

// input
void input(ifstream &InFile, string &line, BankerAlgorithm &banker)
{
    stringstream ss(line);
    string type, ID;
    ss >> ID >> type;
    if (type == "in")
    {
        vector<int> max_claim;
        while (getline(ss, line, ','))
            max_claim.push_back(stoi(line));

        banker.InResources(ID, max_claim);
    }
    else if (type == "want")
    {
        // release 'out'
        ss >> type;
        banker.wantoutResources(ID);
    }
    else if (type == "request")
    {
        vector<int> request;
        while (getline(ss, line, ','))
            request.push_back(stoi(line));

        banker.requestResources(ID, request);
    }
}

int main(int argc, char const *argv[])
{
    ifstream InFile(argv[1]);
    string line;
    getline(InFile, line);
    stringstream ss(line);

    vector<int> Available;
    int resource;

    // Max available resources
    while (getline(ss, line, ','))
        Available.push_back(stoi(line));

    BankerAlgorithm banker(Available);

    while (getline(InFile, line))
        input(InFile, line, banker);

    banker.output();

    return 0;
}
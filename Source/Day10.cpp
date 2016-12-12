#include "Days.h"
#include <regex>
#include <vector>

const size_t INVALID = ((size_t)-1);
static vector<struct Data> dataArray = vector<struct Data>();
static size_t dataIdPart1 = INVALID;

struct Data
{
    Data() : 
        id(0)
    {}

    Data(size_t inId, string inDataType) :
        id(inId), dataType(inDataType)
    {}

    size_t Id() const { return id; }
    string DataType() const { return dataType;}
    size_t LowVal() const {return(values.front()); }
    size_t HighVal() const { return(values.back()); }
    void ClearValues() { values.clear(); }
    size_t ValueCount() const { return values.size(); }

    void AddValue(size_t value)
    {
        values.push_back(value);
        sort(values.begin(), values.end());
    }

private:
    size_t id;
    string dataType;
    vector<size_t> values;
};

Data& GetData(size_t id, const string& dataType)
{
    for(Data& data : dataArray)
    {
        if(data.Id() == id && data.DataType() == dataType)
        {
            return data;
        }
    }

    dataArray.push_back(Data(id, dataType));
    return dataArray.back();
}

template <>
void Run<Day10>(Part part, istream& is, ostream& os)
{
    const regex RX_GOES{ R"(value (\d+) goes to (\w+) (\d+))" };
    const regex RX_GIVES{ R"((\w+) (\d+) gives low to (\w+) (\d+) and high to (\w+) (\d+))" };

    const size_t compareValLow = 17;
    const size_t compareValHigh = 61;

    if(dataArray.empty())
    {
        vector<string> instructions;

        string arg;
        while(getline(is, arg))
        {
            instructions.push_back(arg);
        }

        string instruction;
        while(instructions.size() > 0)
        {
            string instruction = instructions.front();

            smatch match;
            if(regex_match(instruction, match, RX_GOES))
            {
                size_t id = stoi(match.str(3));
                string type = match.str(2);
                size_t value = stoi(match.str(1));

                Data& data = GetData(id, type);
                data.AddValue(value);
            }
            else if(regex_match(instruction, match, RX_GIVES))
            {
                string mainType = match.str(1);
                size_t mainId = stoi(match.str(2));
                string lowType = match.str(3);
                size_t lowId = stoi(match.str(4));
                string highType = match.str(5);
                size_t highId = stoi(match.str(6));

                Data& dataMain = GetData(mainId, mainType);
                if(dataMain.ValueCount() > 1)
                {
                    size_t lowVal = dataMain.LowVal();
                    size_t highVal = dataMain.HighVal();
                    dataMain.ClearValues();

                    if(lowVal == compareValLow && highVal == compareValHigh)
                    {
                        dataIdPart1 = mainId;
                    }

                    Data& dataLow = GetData(lowId, lowType);
                    dataLow.AddValue(lowVal);

                    Data& dataHigh = GetData(highId, highType);
                    dataHigh.AddValue(highVal);
                }
                else
                {
                    instructions.push_back(instruction);
                }
            }

            instructions.erase(instructions.begin());
        }
    }

    switch(part)
    {
        case Part01:
            os << "Bot " << dataIdPart1 << " is comparing values " << compareValLow << " and " << compareValHigh << endl;
            break;
        case Part02:
            Data out0 = GetData(0, "output");
            Data out1 = GetData(1, "output");
            Data out2 = GetData(2, "output");
            size_t product = out0.HighVal() * out1.HighVal() * out2.HighVal();
            os << "Product of outputs " << out0.Id() << ", " << out1.Id() << ", " << out2.Id() << ": " << product << endl;
            break;
    }
}
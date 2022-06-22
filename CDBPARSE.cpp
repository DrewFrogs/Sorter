#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char** argv) {
    string line;
    ifstream myfile (argv[1]);
    if (myfile.is_open()){
		//message to the user
		cout << "CDB Table generation started." << '\n';
		
		int check = 0;
		int hasScope = 0;
		int hasAccess = 0;
		string procedureName;
		ofstream outfile;
		outfile.open("CDB_Table.txt");
		outfile << "LABEL;SCOPE;NAME;INTERFACE_COMPILE_STATUS;BODY_COMPILE_STATUS;CLASS;ACCESS;INTERFACE;TYPE;NUMBER SCALE FACTORS;SCALE" << '\n';
        while ( getline (myfile,line) ){
			if (check == 1){
				if (line.find("BODY_COMPILE_STATUS") != std::string::npos){
					//Fine word and print it
					string bodyStatus;
					bodyStatus = line.substr(line.find('"')+1);
					bodyStatus.pop_back();
					outfile << ';' << bodyStatus << '\n';
					check = 0;
				}
				else if (line.find("BODY_COMPILE_DATE") == std::string::npos && line.find("BODY_COMPILE_TIME") == std::string::npos){
					outfile << '\n';
					check = 0;
				}
			}
            if (line.find("UNIT_NAME") != std::string::npos){
				//Fine word and print it
				string unitName;
				unitName = line.substr(line.find('"')+1);
				unitName.pop_back();
				procedureName = unitName;
			    outfile << "UNIT_NAME;" << " ;" << unitName << ';';
				hasAccess = 0;
			}
			else if (line.find("INTERFACE_COMPILE_STATUS") != std::string::npos){
				//Fine word and print it
				string interStatus;
				interStatus = line.substr(line.find('"')+1);
				interStatus.pop_back();
			    outfile << interStatus;
				check = 1;
			}
			else if (line.find(".SCOPE") != std::string::npos){
				//Fine word and print it
				string scope;
				string type;
				scope = line.substr(line.find('"')+1);
				scope.pop_back();
				if(line.find("TYPE.REF_DATA") != std::string::npos){
					type = line.substr(line.find("REF_DATA"));
					type = type.substr(0, type.find(".", 0));
					outfile << type << ';' << scope << ';';
					hasScope = 1;
				}
				else if(line.find("TYPE.NUM_VISIBLE_ITEM") != std::string::npos){
					type = line.substr(line.find("NUM_VISIBLE_ITEM"));
					type = type.substr(0, type.find(".", 0));
					outfile << type << ';' << scope << ';';
					hasScope = 1;
				}
				else{
					outfile << scope << ';';
				}
			}
			else if (line.find(".NAME") != std::string::npos){
				//Fine word and print it
				string name;
				string type;
				name = line.substr(line.find('"')+1);
				name.pop_back();
				if(line.find("NUM_PROCEDURE_PARAMETER") != std::string::npos){
					type = line.substr(line.find("NUM_PROCEDURE_PARAMETER"));
					type = type.substr(0, type.find(".", 0));
					outfile << type << ';' << procedureName << ';' << name << ';';
				}
				else if (line.find("GEN_DATA") != std::string::npos){
					type = line.substr(line.find("GEN_DATA"));
					type = type.substr(0, type.find(".", 0));
					outfile << type << ';' << procedureName << ';' << name << ';';
				}
				else if (line.find("SUBUNITS") != std::string::npos){
					type = line.substr(line.find("SUBUNITS"));
					type = type.substr(0, type.find(".", 0));
					outfile << type << ';' << procedureName << ';' << name << '\n';
				}
				else if (line.find("WITH_PACKAGE") != std::string::npos){
					type = line.substr(line.find("WITH_PACKAGE"));
					type = type.substr(0, type.find(".", 0));
					if(line.find("VISIBILITY") != std::string::npos){
						outfile << "VISIBILITY: " << name << '\n';
					}
					else{
						outfile << type << ';' << procedureName << ';' << name << ", ";
					}
					
				}
				else if (hasScope == 0){
					if(line.find("TYPE.REF_DATA") != std::string::npos)
					type = line.substr(line.find("REF_DATA"));
					type = type.substr(0, type.find(".", 0));
					outfile << type << ';' << procedureName << ';' << name << ";";
				}
				else{
					outfile << name << ';';
				}
				hasScope = 0;
			}
			else if (line.find(".TYPE") != std::string::npos){
				//Fine word and print it
				string typeName;
				typeName = line.substr(line.find('"')+1);
				typeName.pop_back();
			    outfile << " ;" << typeName;
				if (typeName == "bool"){
					outfile << '\n';
				}
				else{
					outfile << ';';
				}
			}
			else if (line.find("REF_SCALE") != std::string::npos){
				//Fine word and print it
				string scale;
				scale = line.substr(line.find('"')+1);
				scale.pop_back();
			    outfile << scale << '\n';
			}
			else if (line.find(".SCALE") != std::string::npos){
				//Fine word and print it
				string scale;
				scale = line.substr(line.find('"')+1);
				scale.pop_back();
			    outfile << " ;" << scale << '\n';
			}
			else if (line.find(".CLASS") != std::string::npos){
				//Fine word and print it
				string Class;
				Class = line.substr(line.find('"')+1);
				Class.pop_back();
			    outfile << " ; ;" << Class << ';';
			}
			else if (line.find(".ACCESS") != std::string::npos){
				//Fine word and print it
				string access;
				access = line.substr(line.find('"')+1);
				access.pop_back();
			    outfile << access << ';';
				hasAccess = 1;
			}
			else if (line.find("INTERFACE_CLASS") != std::string::npos){
				//Fine word and print it
				string interClass;
				interClass = line.substr(line.find('"')+1);
				interClass.pop_back();
			    outfile << " ;" << interClass << '\n';
			}
			else if (line.find(".INTERFACE") != std::string::npos){
				//Fine word and print it
				string inter;
				inter = line.substr(line.find('"')+1);
				inter.pop_back();
				if(hasAccess == 0){
					outfile << " ;";
				}
			    outfile << inter << ';';
				hasAccess = 0;
			}
			else if (line.find("NUM_OF_SCALE_FACTORS") != std::string::npos){
				//Fine word and print it
				string scaleNum;
				scaleNum = line.substr(line.find('"')+1);
				scaleNum.pop_back();
			    outfile << " ;" << scaleNum;
				if (scaleNum == "0"){
					outfile << '\n';
				}
				else{
					outfile << ';';
				}
			}
        }
        myfile.close();
		outfile.close();
    }
    else{
		cout << "Unable to open file" << '\n';
		return 0;
	}
	
	//message to the user
	cout << "Finished generating CDB Table." << '\n';
	cout << "CDB Set/Used table generation started." << '\n';
	
	//Begin by opening the newly created file to create the set/use table
	ifstream readFile ("CDB_Table.txt");
    if (readFile.is_open()){
		string name;
		string scope;
		ofstream outTable;
		outTable.open("CDB_Set_Use_Table.txt");
		outTable << "Parameter;Set;Used" << '\n';
		while ( getline (readFile,line) ){
			if (line.find("written") != std::string::npos && line.find("REF_DATA") != std::string::npos){
				name = line.substr(line.find(';')+1);
				scope = name.substr(0, name.find(";", 0));
				name = name.substr(name.find(';')+1);
				name = name.substr(0, name.find(";", 0));
				outTable << name << ';' << scope << ';';
				if (line.find("read_before_written") != std::string::npos || line.find("written_before_read") != std::string::npos){
					outTable << scope << '\n';
				}
				else{
					outTable << '\n';
				}
			}
			else if (line.find("read") != std::string::npos && line.find("REF_DATA") != std::string::npos && ((line.find("read_before_written") == std::string::npos) || (line.find("written_before_read") == std::string::npos))){
				name = line.substr(line.find(';')+1);
				scope = name.substr(0, name.find(";", 0));
				name = name.substr(name.find(';')+1);
				name = name.substr(0, name.find(";", 0));
				outTable << name << "; ;" << scope << '\n';
			}
		}
		readFile.close();
		outTable.close();
    }
    else{
		cout << "Unable to open file" << '\n';
		return 0;
	}
	cout << "Finished generating CDB Set/Used Table." << '\n';
	return 0;
}
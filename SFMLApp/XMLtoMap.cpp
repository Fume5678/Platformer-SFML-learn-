#include "XMLtoMap.h"
#include <iostream>

FParserXML::FParserXML(std::string LvlName)
{
	// ����
	std::ifstream ifsLvl;
	// ������ �������� ����
	std::string line;
	// ���� � ����� ������
	std::string LvlPath = LevelPath;
	// ������ ��������, ���� true - ��������� �������� �� ����, ����� �� ���������
	bool bCollis = false;

	///////// ������� ����� ������ \\\\\\\\\\\

	ifsLvl.open(LvlPath + "level_" + LvlName + ".tmx");

	if (!ifsLvl.is_open())
		printf("XML Parser >> Lvl: \nFile opening faled");

	this->LvlName = LvlName;

	while (!ifsLvl.eof()) {
		// ���������� �� �����
		getline(ifsLvl, line);

		std::string::size_type FindRes = std::string::npos;

		// ����� �����
		FindRes = line.find("<map");
		if (FindRes != std::string::npos) {
			// �������� �� ���� ������� �����
			LevelStruct.LvlSize.x = std::stoi(getValueByTag(line, "width"));
			LevelStruct.LvlSize.y = std::stoi(getValueByTag(line, "height"));

			CollisMap.CollisArr = new bool*[LevelStruct.LvlSize.y];
			for (int i = 0; i < LevelStruct.LvlSize.y; i++) {
				CollisMap.CollisArr[i] = new bool[LevelStruct.LvlSize.x];
				for (int j = 0; j < LevelStruct.LvlSize.x; j++) {
					CollisMap.CollisArr[i][j] = false;
				}
			}
		}	

		// ����� ����������
		FindRes = line.find("<tileset");
		if (FindRes != std::string::npos) {
			LevelStruct.TileSize.x = std::stoi(getValueByTag(line, "tilewidth"));
			LevelStruct.TileSize.y = std::stoi(getValueByTag(line, "tileheight"));
			LevelStruct.AmountTiles = std::stoi(getValueByTag(line, "tilecount"));
		}

		// ����� ����������� ��������
		FindRes = line.find("<image");
		if (FindRes != std::string::npos) {
			LevelStruct.PathToSet = (getValueByTag(line, "source"));
			LevelStruct.TileSetSize.x = std::stoi(getValueByTag(line, "width")) / LevelStruct.TileSize.x;
			LevelStruct.TileSetSize.y = std::stoi(getValueByTag(line, "height")) / LevelStruct.TileSize.y;
		}

		// ����� �����
		FindRes = line.find("<layer");
		if (FindRes != std::string::npos) {
			Layers = AddLayer(Layers, LevelStruct.AmountLayer);
			Layers[LevelStruct.AmountLayer].name = getValueByTag(line, "name");
			// ��������� ������� �������� � ����������� �� ��������� ���� 
			if (getValueByTag(line, "collision") == "true")
				Layers[LevelStruct.AmountLayer].Collision = true;
			else
				Layers[LevelStruct.AmountLayer].Collision = false;

			// ��������� ���������� �����. ����� �������� ��� ���������� ������� �����
			LevelStruct.AmountLayer++;		
			CurrLayer++;								  
			i = 0;										  
			j = 0;										  
		}

		// ����� ������
		FindRes = line.find("<tile");
		if (FindRes != std::string::npos) {
			FindRes = line.find("<tileset");	
			if (FindRes == std::string::npos) {

				Layers[CurrLayer].arr[i][j] = std::stoi(getValueByTag(line, "gid"));

				i = i + ((j + 1) / LevelStruct.LvlSize.x);
				j = (j + 1) % LevelStruct.LvlSize.x;
			}
		}

		// ToDo ������ ��� ��������

	} 

	ifsLvl.close();	
}

FParserXML::~FParserXML()
{
	delete[] Layers;
}


std::string FParserXML::getValueByTag(std::string& line, const char* tag)
{
	// ���������� �������� �� ���� � ������� �����. ���� ��������� ������� ���� � ���������, � ����� ������ ��������� 
	std::string Value = "";
	std::string::size_type res = std::string::npos;

	res = line.find(tag);
	if (res == std::string::npos)
		return "0";

	res = line.find_first_of('\"', res);
	
	for (size_t i = res + 1; line[i] != '\"'; i++) {
		Value.push_back(line[i]);
	}

	return Value;
}

FLayer* FParserXML::AddLayer(FLayer* Layer, const int Amount)
{
	
	if (Amount == 0) {
		Layer = new FLayer[Amount + 1];
	}
	else {
		FLayer* tmp = new FLayer[Amount + 1];


		for (int i = 0; i < Amount; i++) {
			tmp[i] = Layer[i];
		}

		delete[] Layer;

		Layer = tmp;
	}
	
	Layer[Amount].arr.resize(LevelStruct.LvlSize.y);

	for (int i = 0; i < LevelStruct.LvlSize.y; i++) {
		Layer[Amount].arr[i].resize(LevelStruct.LvlSize.x);
	}

	return Layer;
}

const std::string FParserXML::getPathToSet()
{
	return LevelStruct.PathToSet;
}

FVector2i FParserXML::getLvlSize()
{
	return LevelStruct.LvlSize;
}

FVector2i FParserXML::getTileSize()
{
	return LevelStruct.TileSize;
}

FVector2i FParserXML::getTileSetSize()
{
	return LevelStruct.TileSetSize;
}

const FLayer& FParserXML::getLayer(int idLayer)
{
	return Layers[idLayer];
}

const FCollisMap FParserXML::getCollisMap()
{
	return CollisMap;
}

const int FParserXML::getAmountLayer() {
	return LevelStruct.AmountLayer;
}

const int FParserXML::getLayerElement(int idLayer, int i, int j)
{
	return Layers[idLayer].arr[i][j];
}

const bool FParserXML::getLayerCollis(int idLayer)
{
	return Layers[idLayer].Collision;
}




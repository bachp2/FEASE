#pragma once
#include <string>
#include <unordered_map>
#include <fstream>
#include <vector>

struct Character {
	int id, x, y, width, height, xoffset, yoffset, xadvance;
};

struct CharacterQuad {
	float x0, x1, y0, y1; //quad coords
	float s0, s1, t0, t1; //texture coords
};

struct Font
{
	uint32_t size;
	uint32_t atlasWidth, atlasHeight;
	int padding, lspacing;
	Texture texture;
	std::string file_name, fface;
	std::unordered_map<int, Character> characters;
};

inline void static get_char_quad(CharacterQuad* q, Character& chr, float px, float py)
{
	//if (!chr) return;
	q->x0 = px + chr.xoffset; 
	q->y0 = py + chr.yoffset;
	q->x1 = px + chr.width + chr.xoffset;
	q->y1 = py + chr.height + chr.yoffset;

	q->s0 = chr.x*1.0f / 128;
	q->s1 = (chr.x + chr.width)*1.0f / 128;
	q->t0 = chr.y*1.0f / 128;
	q->t1 = (chr.y + chr.height)*1.0f / 128;
}

int str2num(std::string::const_iterator& it, std::string& word);
inline void static parse_bm_font_descriptor(const char * file_path, Font* font)
{
	if(!checkIfFileExist(file_path)){
		printf("ERROR: font descriptor not found\n");
		return;
	}

	std::ifstream infile(file_path);
	char ch;
	std::string word;
	std::string var;
	int num_value = 0;
	int parsing = 1;//1: numeric, 2: array, 3: string
	// 1 to parse variable by default, 2 string value, 3 numeric value
	int i = 0;
	std::vector<int> ids, xs, ys, ws, hs, xoffs, yoffs, xads;
	while(infile.get(ch))
	{	
		if (ch == ' ' || ch == '\n') 
		{
			for (auto it = word.cbegin(); it != word.cend(); ++it) {
				if(*it == '='){
					it++;
					num_value = str2num(it, word);
					break;
				}
				var += *it;
			}
			
			if(!var.empty()) 
			{
				//printf("%s = %d\n", var.c_str(), num_value);
				if (var == "id") ids.push_back(num_value);
				else if(var == "x") xs.push_back(num_value);
				else if (var == "y") ys.push_back(num_value);
				else if (var == "width") ws.push_back(num_value);
				else if (var == "height") hs.push_back(num_value);
				else if (var == "xoffset") xoffs.push_back(num_value);
				else if (var == "yoffset") yoffs.push_back(num_value);
				else if (var == "xadvance") xads.push_back(num_value);
				else if (var == "lineHeight") font->lspacing = num_value;
				else if (var == "size") font->size = num_value;
			}
			word.clear(); var.clear(); num_value = 0;
			continue;
		}
		word += ch;
	}

	ids.erase(ids.begin());
	/*printf("%d\n", ids.size());
	printf("%d\n", xs.size());
	printf("%d\n", ys.size());
	printf("%d\n", ws.size());
	printf("%d\n", hs.size());
	printf("%d\n", xoffs.size());
	printf("%d\n", yoffs.size());
	printf("%d\n", xads.size());*/
	
	font->characters.reserve(xs.size());
	for (auto i = 0; i < xs.size(); ++i){
		Character chr;
		chr.id = ids[i];
		chr.x = xs[i];
		chr.y = ys[i];
		chr.width = ws[i];
		chr.height = hs[i];
		chr.xoffset = xoffs[i];
		chr.yoffset = yoffs[i];
		chr.xadvance = xads[i];
		font->characters[chr.id] = chr;
	}
	//Character chr = (*characters)['F'];
	/*printf("id:%d, x:%d, y:%d, w:%d, h:%d, xoffset:%d, yoffset:%d, xadvance:%d\n", chr.id, chr.x, chr.y, chr.width, chr.height, chr.xoffset, chr.yoffset, chr.xadvance);*/
	//printf("size:%d, line h:%d, spacing: %d, %d\n", font->size, font->lspacing);
	infile.close();
}

inline static int str2num(std::string::const_iterator& it, std::string& word){
	int sign = 1;
	int num_value = 0;
	if(*it == '-'){
		sign = -1;
		it++;
	}
	for(auto it1 = it; it1 != word.cend(); ++it1){
		if(*it1 == ','){ 
			num_value = 0;
			break;
		}
		num_value = num_value*10 + (*it1-'0');
	}
	return num_value*sign;
}
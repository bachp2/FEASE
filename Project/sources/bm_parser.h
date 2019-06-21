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
inline void static get_char_quad(CharacterQuad* q, Character& chr, float px, float py)
{
	//if (!chr) return;
	q->x0 = px; q->y0 = py;
	q->x1 = px + chr.width;
	q->y1 = py + chr.height;

	q->s0 = chr.x*1.0f / 128;
	q->s1 = (chr.x + chr.width)*1.0f / 128;
	q->t0 = chr.y*1.0f / 128;
	q->t1 = (chr.y + chr.height)*1.0f / 128;
}

inline void static parse_bm_font_descriptor(const char * file_path, std::unordered_map<int, Character>* characters)
{
	std::ifstream infile(file_path);
	char ch;
	std::string word;
	std::string var;
	int num_value = 0;
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
					for(auto it1 = it; it1 != word.cend(); ++it1){
						num_value = num_value*10 + (*it1-'0');
					}
					break;
				}
				var += *it;
			}
			
			if(!var.empty()) 
			{
				printf("%s = %d\n", var.c_str(), num_value);
				if (var == "id") ids.push_back(num_value);
				else if(var == "x") xs.push_back(num_value);
				else if (var == "y") ys.push_back(num_value);
				else if (var == "width") ws.push_back(num_value);
				else if (var == "height") hs.push_back(num_value);
				else if (var == "xoffset") xoffs.push_back(num_value);
				else if (var == "yoffset") yoffs.push_back(num_value);
				else if (var == "xadvance") xads.push_back(num_value);
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
	
	characters->reserve(xs.size());
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
		(*characters)[chr.id] = chr;
	}
	Character chr = (*characters)['F'];
	/*printf("id:%d, x:%d, y:%d, w:%d, h:%d, xoffset:%d, yoffset:%d, xadvance:%d\n", chr.id, chr.x, chr.y, chr.width, chr.height, chr.xoffset, chr.yoffset, chr.xadvance);*/
	infile.close();
}
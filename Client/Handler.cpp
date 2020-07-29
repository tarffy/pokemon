#include "Handler.h"
#include <qdebug.h>
#include <qmessagebox.h>
Handler::Handler(QObject *parent)
	: QObject(parent)
{
}

Handler::~Handler()
{
}

pokemon_base * Handler::string_to_pokemon(const QString & str)
{
	pokemon_base *res;
	QStringList pokemon_args = str.split(",");
	//影魔,1,unique_id,pokemon_id,500,130,40,100,15,10,2,0,1,0,200
	string name = pokemon_args.at(0).toStdString();
	vector<int>id{ pokemon_args.at(2).toInt(),pokemon_args.at(3).toInt() };
	vector<int> atti;
	for (int i = 0; i < 8; i++)atti.push_back(pokemon_args.at(4 + i).toInt());
	vector<int> levels;
	for (int i = 0; i < 3; i++)levels.push_back(pokemon_args.at(12 + i).toInt());
	if (pokemon_args.at(1).toInt() == 1)res = new pokemon_r(name,id, atti, levels);
	return res;
}

void Handler::handle_str_from_socket(const QString & str)
{
	QStringList strs = str.split("^()");
	for (int i = 0; i < strs.size(); i++) {
		if (strs.at(i).length() == 0)continue;
		QStringList list = strs.at(i).split("****");
		QString mode = list.at(0);
		if (mode == "register" || mode == "login") {
			if (list.at(1) == "success") {
				emit register_or_login_success(list.at(2));
			}
			else {
				emit register_or_login_fail(list.at(2));
			}
		}
		else if (mode == "query_player") {
			emit query_success(list.at(1));
		}
		else if (mode == "information") {
			emit socket_information(list.at(1));
		}
		else if (mode == "query_pokemon") {
			QStringList pokemons = list.at(1).split("###");
			QStringList bag_name;
			QStringList store_name;
			if (pokemons.at(0) != "-1") {
				QStringList pokemon_bag = pokemons.at(0).split("$$");
				for (int i = 0; i < pokemon_bag.size(); ++i) {
					player.put_pokemon_in_bag(string_to_pokemon(pokemon_bag.at(i)));
				}
			}
			if (pokemons.at(1) != "-1") {
				QStringList pokemon_store = pokemons.at(1).split("$$");
				for (int i = 0; i < pokemon_store.size(); ++i) {
					player.put_pokemon_in_store(string_to_pokemon(pokemon_store.at(i)));
				}
			}
			QString names = QString::fromStdString(player.get_all_names()) ;

			emit pokemon_info_ready(names);
			
		}
		else if (mode=="query_player_pokemon") {//精灵以###隔开 名字登记,unique id
			emit player_pokemon_ready(list.at(1));
		}
		else if (mode == "add_pokemon") {
			player.put_pokemon_in_store(string_to_pokemon(list.at(1)));
			QString names = QString::fromStdString(player.get_all_names());
			emit pokemon_info_ready(names);
		}
	}
}

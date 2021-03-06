// fingerd.c

#include <net/dns.h>

void create() { seteuid( getuid() ); }

string age_string(int time)
{
	int month, day, hour, min, sec;

	sec = time % 60;
	time /= 60;
	min = time % 60;
	time /= 60;
	hour = time % 24;
	time /= 24;
	day = time % 30;
	month = time / 30;
	return (month?month + "m":"") + (day?day + "d":"") + 
	(hour?hour + "h":"") + min + "m";
}

string finger_all()
{
	object *ob;
	string msg, fip;
	int i;

	ob = users();
	msg = "";
	if ( !wizardp(this_player()) )  // player finger
	{
		for(i=0; i<sizeof(ob); i++) {
			if( this_player() && !this_player()->visible(ob[i]) ) continue;
			msg = sprintf("%s%-14s%-14s%-14s\n",
				msg, ob[i]->query("name"), ob[i]->query("id"),
				query_idle(ob[i]) + "s");
		}
		return "£ 署唸蛤論勧\n" + 
		"！！！！！！！！！！！！！！！！！！\n"
		+ "侖兆          嬖催          窟岐\n" +
		"！！！！！！！！！！！！！！！！！！\n"
		+ msg +
		"！！！！！！！！！！！！！！！！！！\n";
	} else  	// wizard finger
	{
		for(i=0; i<sizeof(ob); i++) {
			if( this_player() && !this_player()->visible(ob[i]) ) continue;
			msg = sprintf("%s%-14s%-14s%-14s%-7s%s\n",
				msg, ob[i]->query("name"), ob[i]->query("id"),
				age_string( (int)ob[i]->query("mud_age")), 
				query_idle(ob[i]) + "s", query_ip_name(ob[i]));
		}
		return "£ 署唸蛤論勧\n" + 
		"！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！\n"
		+ "侖兆          嬖催          定槍          窟岐   銭??\n" +
		"！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！\n"
		+ msg +
		"！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！\n";
	}
}
	
string finger_user(string name)
{
	object ob, body;
	string msg, mud;
	int public_flag;

/*	if( sscanf(name, "%s@%s", name, mud)==2 ) {
		GFINGER_Q->send_finger_q(mud, name, this_player(1));
		return "利揃峺綜勧僕狛殻辛嬬俶勣匯乂扮寂??萩不昨。\n";
	}
*/
	ob = new(LOGIN_OB);
	ob->set("id", name);
	if( !ob->restore() ) return "短嗤宸倖螺社。\n";
	
	
	if ( !wizardp(this_player()) )  // player finger
	{
	
		if( objectp(body = find_player(name)) && geteuid(body)==name )
		{
			public_flag = body->query("env/public")?1:0;
		} else 
		{
			body = LOGIN_D->make_body(ob);
			if( !body->restore() ) return "短嗤宸倖螺社。\n";
			public_flag = body->query("env/public")?1:0;
			destruct(body);
		}		
		
		msg =  sprintf("\n哂猟旗催??\t%s\n侖    兆??\t%s\n幡?浹伴僑?\t%s\n"
			"窮徨喨周仇峽??\t%s\n貧肝銭?滷?\t%s\n\n",
			ob->query("id"),
			ob->query("name"),
			SECURITY_D->get_status(name),
			public_flag?ob->query("email"):"音御盆低",
			ctime(ob->query("last_on"))
		);
		if( objectp(body = find_player(name)) && geteuid(body)==name ) {
			msg += sprintf("\n%s朕念屎壓銭?炒弌?\n", body->name(1));
		}
	} else  	// wizard finger
	{
		msg =  sprintf("\n哂猟旗催??\t%s\n侖    兆??\t%s\n幡?浹伴僑?\t%s\n"
			"窮徨喨周仇峽??\t%s\n貧肝銭?澣慟沓?\t%s( %s )\n\n",
			ob->query("id"),
			ob->query("name"),
			SECURITY_D->get_status(name),
			ob->query("email"),
			ob->query("last_from"),
			ctime(ob->query("last_on"))
		);
		if( objectp(body = find_player(name)) && geteuid(body)==name ) {
			msg += sprintf("\n%s朕念屎壓貫 %s 銭?炒弌?\n", body->name(1),
				query_ip_name(body));
		}
	}	
	
	destruct(ob);
	return msg;
}

varargs string remote_finger_user(string name, int chinese_flag)
{
	object ob, body;
	string msg;

	ob = new(LOGIN_OB);
	ob->set("id", name);
	if( !ob->restore() )
		return chinese_flag ? "短嗤宸倖螺社。\n" : "No such user.\n";
	if( chinese_flag ) msg =  sprintf(
		"\n哂猟旗催??\t%s\n侖    兆??\t%s\n幡?浹伴僑?\t%s\n"
		"窮徨喨周仇峽??\t%s\n貧肝銭?澣慟沓?\t%s( %s )\n\n",
		ob->query("id"),
		ob->query("name"),
		SECURITY_D->get_status(name),
		ob->query("email"),
		ob->query("last_from"),
		ctime(ob->query("last_on"))
	);
	else msg =  sprintf(
		"\nName\t: %s\nStatus\t: %s\nEmail\t: %s\nLastOn\t: %s( %s )\n\n",
		capitalize(ob->query("id")),
		SECURITY_D->get_status(name),
		ob->query("email"),
		ob->query("last_from"),
		ctime(ob->query("last_on"))
	);
	if( body = find_player(name) ) {
		if( !this_player() || this_player()->visible(body) )
			msg += chinese_flag ?
				("\n" + ob->query("name") + "朕念屎壓?濾蓮?\n"):
				("\n" + capitalize(name) + " is currently connected.\n");
	}

	destruct(ob);
	return msg;
}

object acquire_login_ob(string id)
{
	object ob;

	if( ob = find_player(id) ) {
		// Check if the player is linkdead
		if( ob->query_temp("link_ob") )
			return ob->query_temp("link_ob");
	}
	ob = new(LOGIN_OB);
	ob->set("id", id);
	return ob->restore() ? ob : 0;
}

string get_killer()
{
	int i;
	string msg;
	object *ob = users();

	msg = "";
	for (i = 0; i < sizeof(ob); i++)
		if ((int)ob[i]->query_condition("killer"))
			msg += (ob[i]->short() + "\n");
	if (msg == "")
		return "云廓嵶芦措挫。\n";
	else
		return "?嶬擶廠杯?壓而鎮參和繁係??\n\n" + msg;
}

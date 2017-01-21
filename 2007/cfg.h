#pragma once

#include <stdio.h>

class list{
  struct data{
    void* info;
    data* next;
  };

  data*  head;
  data*  current;

public:
  list(){
    head = current = 0;
     }

  ~list(){
    current = head;
    while(!is_empty())
      rem_data();
     }


  void add_data(void* new_data){

    data* temp;

       temp = new data;
    temp->next = 0;
    temp->info = new_data;
    if(!is_empty()){
      end();
      current->next = temp;
    }
        else{
      head = temp;
    }
    current = temp;
  };

  void add_atpos(void* new_data){
    data* temp = new data;
    temp->info = new_data;
    if(is_empty()){
      temp->info = new_data;
      temp->next = 0;
      head = temp;
      current = head;
    }
    else{
      temp->info = current->info;
      temp->next = current->next;
      current->info = new_data;
      current->next = temp;
    }
  };

  void rem_data(){

    data* temp;

    if(!is_empty()){
      if(is_at_home())
        head = head->next;
              else{
          temp = head;
        while(temp->next != current)
          temp = temp->next;
                   temp->next = current->next;
      }
      delete current;
      home();
    }
     };

  void* get_data(){
    if( (!is_empty()) && (is_valid()) )
      return(current->info);
       else
      return 0;
     };

  void home(){
    current = head;
  };

  char is_at_home(){
    if(!is_empty())
      if(current == head)
        return 1;
           return 0;
  };

  char is_at_end(){
    if(!is_empty()){
      if(!current)
        return 1;
      if(!current->next)
        return 1;
    }
      return 0;
  };

      char is_empty(){
    if(head)
      return 0;
         return 1;
     };

  char is_valid(){
    if(current)
      return 1;
       return 0;
  };

  void end(){
    home();
    if(!is_empty())
      while(next());
    else
        current = 0;
     };

  int next(){
    if(!is_empty())
      if(current->next){
        current = current->next;
        return 1;
      }
    return 0;
  };
};

struct config_entry{
    char *name;
    char *val;
};

char* find_value(list* parms, char* name)
{
  char*      value;
  config_entry*  data;

  if(parms->is_empty())
    return NULL;
  parms->home();
  do{
    data = (config_entry*) parms->get_data();
    if(!strcmp(data->name,name)){
      value = (char*) malloc(strlen(data->val)+1);
      strcpy(value,data->val);
      return value;
    }
  }while(parms->next());
  return "";
}

list* read_config(FILE* fp){

  list*  parms = new list;
  bool ende=false;
  char*  line = new char[200];
  config_entry*  new_item;
  char   *parse,
      *parse2;
  int    space;

  while(!feof(fp))
  {
    parse = fgets(line,199,fp);        //Read a single line from file
	if(!(parse==0 || *parse=='#' || *parse=='\n')){    //Allow for blank lines and comment lines
        new_item = new config_entry;
        space = 0;
        while(*parse != '=')
		{
			ende=(*parse == '\n');
			if(ende)
				break;

              if(*parse == ' ')
                 space++;
               parse++;
           }

           *(parse - space) = 0;
        new_item->name = new char[strlen(line)+1];
        strcpy(new_item->name,line);
        space = 0;
		parse++;
		if(!ende)
		{
			parse2 = parse;
			while((*parse != '\n') && (*parse != 0)){
			if(*parse == ' ')
				space++;
			parse++;
			}
		}
			*(parse - space) = 0;
			new_item->val = new char[strlen(parse2)+1];
			strcpy(new_item->val,parse2);

		parms->add_data((void*) new_item);
      }
  }

  return parms;
}
bool bool_value(list* parms, char* name)
{
	return find_value(parms, name)!="";
}


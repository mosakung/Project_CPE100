#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Menu
{
    char ID[256];
    char Name[256];
    double Price;
};

struct Order
{
    char ID[256];
    int Quantity;
};

int CountData = 0;
int CountOrder = 0;

struct Menu * menu;
struct Order * ord;

void Load_File(char *NameFile)
{
    CountData = 0;
    char input[256];

    FILE *FP = fopen(NameFile, "r");

    if (!FP)
    {
        printf("Not Found %s\n", NameFile);
        return;
    }

    struct Menu * temp = menu;

    while(!feof(FP))
    {
        fgets(input,255,FP);
        sscanf(input,"%[^,],%[^,],%lf",&temp->ID, &temp->Name, &temp->Price);
        temp += 1;
        CountData += 1;
    }
    CountData--; // warning
    printf("Load %d Data success\n\n",CountData);
    fclose(FP);
}

void SaveMenu(char *NameFile)
{
    struct Menu * temp = menu;

    FILE *FP = fopen(NameFile, "w");

    if (!FP)
    {
        printf("Not Found %s\n", NameFile);
        return;
    }

    for(int i=0; i<CountData; i++)
    {
        fprintf(FP, "%s,%s,%lf\n",(temp+i)->ID,(temp+i)->Name,(temp+i)->Price);
    }
    printf("Save %d Data success\n",CountData);
    fclose(FP);
}

void SaveOrder(char *NameFile)
{
    struct Order * temp = ord;

    FILE *FP = fopen(NameFile, "w");

    if (!FP)
    {
        printf("Not Found %s\n", NameFile);
        return;
    }

    for(int i=0; i<CountData; i++)
    {
        fprintf(FP, "%s,%d\n",(temp+i)->ID,(temp+i)->Quantity);
    }
    printf("Save %d Data success\n",CountOrder);
    fclose(FP);
}

void ShowMenu(struct Menu *temp)
{
    printf("Menu\n");
    printf("ID  || Name\t\t\t      || Price\n");
    for(int i=0; i<CountData; i++)
    {
        printf("%3s || %-30s || %.2lf\n",(temp + i)->ID,(temp + i)->Name,(temp + i)->Price);
    }
}

void AddMenu(struct Menu * temp,char *iD,char *name,double price)
{
    char spos[256] = "true";
    for(int i=0; i<CountData; i++)
    {
        if(strcmp((temp+i)->ID,iD)==0)
        {
            strcpy(spos,"miss");
        }
    }
    if(strcmp(spos,"miss")==0)
    {
        printf("already this ID\n");
        return;
    }
    strcpy((temp+CountData)->ID,iD);
    strcpy((temp+CountData)->Name,name);
    (temp+CountData)->Price = price;
    temp += 1;
    CountData += 1;
}

void DeleteMenu(struct Menu * temp,char *pos)
{
    char spos[256] = "miss";
    int j,k;
    for(int i=0; i<CountData; i++)
    {
        if(strcmp((temp+i)->ID,pos)==0)
        {
            strcpy(spos,pos);
            j = i;
        }
    }
    if(strcmp(spos,"miss")==0)
    {
        printf("not found ID");
        return;
    }
    for(int i=j; i<CountData; i++)
    {
        strcpy((temp+i)->ID,(temp+i+1)->ID);

        strcpy((temp+i)->Name,(temp+i+1)->Name);

        (temp+i)->Price = (temp+i+1)->Price;
    }
    CountData--;
}

double SearchP(char *id)
{
    struct Menu * temp = menu;
    for(int i=0; i<CountData; i++)
    {
        if(strcmp(temp[i].ID,id)==0)
            return temp[i].Price;
    }
    return -1;
}

void AddOrder(struct Order * temp,char *iD,int qua)
{
    int j;
    char spos[256] = "true";
    if(SearchP(iD) == -1)
    {
        printf("Not found ID: %s in Menu\n",iD);
        return;
    }
    for(int i=0; i<CountOrder; i++)
    {
        if(strcmp((temp+i)->ID,iD)==0)
        {
            strcpy(spos,iD);
            j=i;
        }
    }
    if(strcmp(spos,"true")==0)
    {
        strcpy((temp+CountOrder)->ID,iD);
        (temp+CountOrder)->Quantity = qua;
        temp += 1;
        CountOrder += 1;
    }
    else if(strcmp(spos,"true")!=0)
    {
        (temp+j)->Quantity = ((temp+j)->Quantity)+qua;
    }
}

void ShowOrder(struct Order *temp)
{
    printf("Order\n");
    printf("ID  || Quantity\n");
    for(int i=0; i<CountOrder; i++)
    {
        printf("%-3s || %-10d\n",(temp + i)->ID,(temp + i)->Quantity);
    }
}

void DeleteOrder(struct Order * temp,char *pos)
{
    char spos[256] = "miss";
    int j,k;
    for(int i=0; i<CountOrder; i++)
    {
        if(strcmp((temp+i)->ID,pos)==0)
        {
            strcpy(spos,pos);
            j = i;
        }
    }
    if(strcmp(spos,"miss")==0)
    {
        printf("not found ID");
        return;
    }
    for(int i=j; i<CountOrder; i++)
    {
        strcpy((temp+i)->ID,(temp+i+1)->ID);

        (temp+i)->Quantity = (temp+i+1)->Quantity;
    }
    CountOrder--;
}

double Total(struct Order * temp)
{
    double mul=0;
    double total=0;
    if(temp == NULL)
    {
        printf("Order is blank\n");
        return;
    }
    for(int i=0; i<CountOrder; i++)
    {
        mul = SearchP((temp+i)->ID) * (temp+i)->Quantity;
        total = total + mul;
    }
    return total;
}

int split(char *Key, char **token[5])
{
    int count = 0;


    char* temp = strtok(Key, " ");
    while(temp)
    {
        token[count] = temp;
        count++;
        temp = strtok(NULL, " ");
    }
    return count;
}

int main()
{
    menu = (struct Menu*) malloc(100000 * sizeof(struct Menu));
    ord = (struct Order*) malloc(100000 * sizeof(struct Order));

    int exit,state = 0,Mode,Scount,num;
    char input[256],Key[256],len[256];
    char * Token[5];
    double price;

    exit = 0;

    if(menu == NULL || ord == NULL)
    {
        printf("Error! memory not allocated.");
    }
    printf("\n");

    Load_File("menus.txt");

    while(1)
    {
        if(exit == 1)
            break;

        strcpy(input,"");
        strcpy(Token[1],"");
        Mode = 100;

        printf("\n");

        if(state == 0)
            printf("$Menu>");
        else if(state == 1)
            printf("$Order>");

        fgets(input,sizeof(input),stdin);

        Scount = split(input,&Token);

        strcpy(Token[0],strlwr(Token[0]));
        Token[0][strcspn(Token[0], "\n")] = 0;

        printf("\n");

        if(strcmp(Token[0],"load")==0)
            Mode = 1;
        else if(strcmp("show",Token[0])==0)
        {
            if(state == 0)
                Mode = 2;
            else if(state == 1)
                Mode = 12;
        }
        else if(strcmp("menu",Token[0])==0)
        {
            Mode = 3;
            state = 0;
        }
        else if(strcmp("order",Token[0])==0)
        {
            Mode = 3;
            state = 1;
        }
        else if(strcmp("add",Token[0])==0)
        {
            if(state == 0)
                Mode = 4;
            else if(state == 1)
                Mode = 7;
        }
        else if(strcmp("delete",Token[0])==0)
        {
            if(state == 0)
                Mode = 5;
            else if(state == 1)
                Mode = 8;
        }
        else if(strcmp("save",Token[0])==0)
        {
            if(state == 0)
                Mode = 6;
            else if(state == 1)
                Mode = 9;
        }
        else if(strcmp("total",Token[0])==0)
            Mode = 10;
        else if(strcmp("help",Token[0])==0)
            Mode = 13;
        else if(strcmp("exit",Token[0])==0)
            Mode = 14;
        else
            Mode = 11;

        switch(Mode)
        {
        case 1: // load menu
            Token[1][strcspn(Token[1], "\n")] = 0;
            Load_File(Token[1]);
            break;
        case 2: // show menu
            ShowMenu(menu);
            break;
        case 3: // change state
            break;
        case 4: // add menu
            sscanf(Token[3], "%lf", &price);
            Token[1][strcspn(Token[1], "\n")] = 0;
            Token[2][strcspn(Token[2], "\n")] = 0;
            AddMenu(menu,Token[1],Token[2],price);
            break;
        case 5: // delete menu
            Token[1][strcspn(Token[1], "\n")] = 0;
            DeleteMenu(menu,Token[1]);
            break;
        case 6: // save menu
            Token[1][strcspn(Token[1], "\n")] = 0;
            SaveMenu(Token[1]);
            break;
        case 7: // add order
            Token[1][strcspn(Token[1], "\n")] = 0;
            num = atoi (Token[2]);
            if(num<0)
            {
                printf("!quantity is minus\n");
                break;
            }
            AddOrder(ord,Token[1],num);
            ShowOrder(ord);
            break;
        case 8: // delete order
            Token[1][strcspn(Token[1], "\n")] = 0;
            DeleteOrder(ord,Token[1]);
            ShowOrder(ord);
            break;
        case 9: // save order
            Token[1][strcspn(Token[1], "\n")] = 0;
            SaveOrder(Token[1]);
            break;
        case 10: // total
            printf("Total = %lf\n",Total(ord));
            SaveOrder("orders.txt");
            break;
        case 11: // default
            printf("command '%s' not found\n",Token[0]);
            break;
        case 12: // showorder
            ShowOrder(ord);
            break;
        case 13:
            printf("2 Mode\n");
            printf("- Menu\n");
            printf("- Order\n\n");
            printf("Menu Command\n");
            printf("- load [namefile]\n");
            printf("- show\n");
            printf("- add [id] [name] [price]\n");
            printf("- delete [id]\n");
            printf("- save [namefile]\n");
            printf("- total\n");
            printf("- help\n\n");
            printf("Order Command\n");
            printf("- show\n");
            printf("- add [id] [quantity]\n");
            printf("- delete [id]\n");
            printf("- save [namefile]\n");
            printf("- total\n");
            printf("- help\n");
            printf("\n");
            break;
        case 14:
            exit = 1;
            printf("bye\n");
            break;
        default :
            printf("Im default\n");
        }
    }
}

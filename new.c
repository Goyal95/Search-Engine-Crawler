#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<sys/types.h>
#include<sys/stat.h>
#include"html.h"
#define URL_LENGTH 1000
#define BASE_URL "http://www.chitkara.edu.in/"
#define BASE_LENGTH 27
#define HASH_SIZE 100
#define MAX_URL_PER_PAGE 20

struct urlinfo{
	char* url;
	int vis;
	int depth;
};

struct mainll{
	struct urlinfo* info;
	struct mainll* next;
	struct mainll* prev;
}*start=NULL;

struct hash{
	struct mainll* begin;
	int count;
}h[HASH_SIZE];

void addinfo(char* a,char* b,struct mainll* node){
	int i;
	struct urlinfo* m = malloc(sizeof(struct urlinfo));
	//memset(m,0,(sizeof(struct urlinfo)));
	m->url= malloc(strlen(a)+1);
	//memset(m->url,0,(strlen(a)+1));
	for(i=0;i<strlen(a);i++){
		m->url[i]=a[i];
	}
	m->url[i]='\0';
	m->vis=0;
	m->depth=b[0]-48;
	node->info=m;
}

int genkey(char* a){
	return strlen(a)%100;
}

void hashnode(struct mainll* m, int key){
	if(h[key].count==0){
		h[key].begin=m;
		h[key].count++;
	}
	else
		h[key].count++;
}

void insertll(char* a,char* b){
	int key=genkey(a),dep=b[0]-48,flag=0;
	if(dep<=2){
		struct mainll* m = malloc(sizeof(struct mainll));
		//memset(m,0,sizeof(struct mainll));
		addinfo(a,b,m);
		if(start==NULL){
			start=m;
			m->next=NULL;
			m->prev=NULL;
			hashnode(m,key);
		}
		else{
			if(h[key].count==0){
				start->prev=m;
				m->next=start;
				m->prev=NULL;
				start=m;
			}
			else{
				int i;
				struct mainll* n=h[key].begin;
				int c=h[key].count;
				for(i=0;i<h[key].count;i++){
					if(strcmp(n->info->url,a)==0){
						flag=1;
						break;
					}
					if(c!= 1)
						n=n->next;
					c--;
				}
				if(flag==0){
					if(n->next==NULL){
						n->next=m;
						m->prev=n;
						m->next=NULL;
					}
					else{
						n->next->prev=m;
						m->next=n->next;
						m->prev=n;
						n->next=m;
					}
				}
			}
			if(flag!=1)
				hashnode(m,key);
		}
	}   
}

void setvisited(char* a){
	int key=genkey(a);
	int i;   
	struct mainll* n=h[key].begin;
	for(i=0;i<h[key].count;i++){
		if(strcmp(a,n->info->url)==0)
			break;
		n=n->next;
	}
	n->info->vis=1;
}

void getpage(char *url){
	char urlbuffer[URL_LENGTH+300]={0};   
	strcat(urlbuffer, "wget ");
	strcat(urlbuffer, url);
	strcat(urlbuffer," -O /home/lakshay/haha/temp.txt ");
	system(urlbuffer);
}
   
   
void testDir(char *dir){
	struct stat statbuf;
	if(stat(dir,&statbuf)==-1){
		fprintf(stderr, "-----------------\n");
		fprintf(stderr, "Invalid directory\n");
		fprintf(stderr, "-----------------\n");
		exit(1);
	}
   
      //Both check if there's a directory and if it's writable
	if(!S_ISDIR(statbuf.st_mode)){
		fprintf(stderr, "-----------------------------------------------------\n");
		fprintf(stderr, "Invalid directory entry. Your input isn't a directory\n");
		fprintf(stderr, "-----------------------------------------------------\n");
		exit(1);
	}
   
	if((statbuf.st_mode & S_IWUSR)!=S_IWUSR){
		fprintf(stderr, "------------------------------------------\n");
		fprintf(stderr, "Invalid directory entry. It isn't writable\n");
		fprintf(stderr, "------------------------------------------\n");
		exit(1);
	}
}

int check_page(char *url){
	char buffer[URL_LENGTH + 300]={0};
	strcat(buffer, "wget --spider ");
	strcat(buffer, url);
	if(!system(buffer))
		return 1;
	else
		return 0;
}
   
int checkPARA(int a){
	if(a!=4)
		return 0;
	else
		return 1;
}
   
int checkDEPTH(char a[]){
	int i=a[0];
	int b=a[0]-48;
	if(b>3 || i<48 || i>57)
		return 0;
	else
		return 1;
}
   
int checkURL(char a[]){
	int i;
	for(i=0;i<BASE_LENGTH;i++){
		if(a[i]!=BASE_URL[i]||a[i]=='\0')
			return 0;
	}
	return 1;
}

char* getcURL(FILE* fp1){
	char *u=malloc(URL_LENGTH + 1);
	//memset(u,0,(URL_LENGTH+1));
	char a=fgetc(fp1);
	int i=0;
	while(a!='\n'){
		a=fgetc(fp1);
		if(a=='='){
			while(a!='\n'){
				a=fgetc(fp1); u[i++]=a;
			}
		}
	}
	u[i-1]='\0';
	return u;
}

char getdepth(FILE* fp1){
	char a=fgetc(fp1),d;
	while(a!='\n'){
		a=fgetc(fp1);
		if(a=='='){
			a=fgetc(fp1);
			int ss=a+1;
            d=ss;
            a=fgetc(fp1);
            }
        }
return d;
}


void NormalizeWord(char* word) {
      int i = 0;
      while (word[i]) {
              // NEW
        if (word[i] < 91 && word[i] > 64) // Bounded below so this funct. can run on all urls
          // /NEW
          word[i] += 32;
        i++;
      }
}

int NormalizeURL(char* URL)
{
      int len = strlen(URL);
      if (len <= 1 )
        return 0;
      //! Normalize all URLs.
      if (URL[len - 1] == '/')
      {
        URL[len - 1] = 0;
        len--;
      }
      int i, j;
      len = strlen(URL);
      //! Safe check.
      if (len < 2)
        return 0;
      //! Locate the URL's suffix.
      for (i = len - 1; i >= 0; i--)
        if (URL[i] =='.')
          break;
      for (j = len - 1; j >= 0; j--)
        if (URL[j] =='/')
          break;
      //! We ignore other file types.
      //! So if a URL link is to a file that are not in the file type of the following
      //! one of four, then we will discard this URL, and it will not be in the URL list.
      if ((j >= 7) && (i > j) && ((i + 2) < len))
      {
        if ((!strncmp((URL + i), ".htm", 4))
            ||(!strncmp((URL + i), ".HTM", 4))
            ||(!strncmp((URL + i), ".php", 4))
            ||(!strncmp((URL + i), ".jsp", 4))
            )
    {
          len = len; // do nothing.
        }
        else
        {
          return 0; // bad type
        }
      }
      return 1;
    }
   
void removeWhiteSpace(char* html){
      int i;
      char *buffer = malloc(strlen(html)+1), *p= malloc (sizeof(char)+1);
      memset(buffer,0,strlen(html)+1);
      for (i=0;html[i];i++)
      {
        if(html[i]>32)
            {
              sprintf(p,"%c",html[i]);
              strcat(buffer,p);
            }
      }
      strcpy(html,buffer);
      free(buffer); free(p);
}

int GetNextURL(char* html, char* urlofthispage, char* result, int pos){
      char c;
      int len, i, j;
      char* p1;  //!< pointer pointed to the start of a new-founded URL.
      char* p2;  //!< pointer pointed to the end of a new-founded URL.
   
      // NEW
      // Clean up \n chars
      if(pos == 0) {
        removeWhiteSpace(html);
      }
      // /NEW
   
      // Find the <a> <A> HTML tag.
      while (0 != (c = html[pos]))
      {
        if ((c=='<') &&
            ((html[pos+1] == 'a') || (html[pos+1] == 'A'))) {
          break;
        }
        pos++;
      }
      //! Find the URL it the HTML tag. They usually look like <a href="www.abc.com">
      //! We try to find the quote mark in order to find the URL inside the quote mark.
      if (c)
      { 
        // check for equals first... some HTML tags don't have quotes...or use single quotes instead
        p1 = strchr(&(html[pos+1]), '=');
       
        if ((!p1) || (*(p1-1) == 'e') || ((p1 - html - pos) > 10))
        {
          // keep going...
          return GetNextURL(html,urlofthispage,result,pos+1);
        }
        if (*(p1+1) == '\"' || *(p1+1) == '\'')
          p1++;
   
        p1++;   
   
        p2 = strpbrk(p1, "\'\">");
        if (!p2)
        {
          // keep going...
          return GetNextURL(html,urlofthispage,result,pos+1);
        }
        if (*p1 == '#')
        { // Why bother returning anything here....recursively keep going...
   
          return GetNextURL(html,urlofthispage,result,pos+1);
        }
        if (!strncmp(p1, "mailto:",7))
          return GetNextURL(html, urlofthispage, result, pos+1);
        if (!strncmp(p1, "http", 4) || !strncmp(p1, "HTTP", 4))
        {
          //! Nice! The URL we found is in absolute path.
          strncpy(result, p1, (p2-p1));
          return  (int)(p2 - html + 1);
        } else {
          //! We find a URL. HTML is a terrible standard. So there are many ways to present a URL.
          if (p1[0] == '.') {
            //! Some URLs are like <a href="../../../a.txt"> I cannot handle this.
        // again...probably good to recursively keep going..
        // NEW
           
            return GetNextURL(html,urlofthispage,result,pos+1);
        // /NEW
          }
          if (p1[0] == '/') {
            //! this means the URL is the absolute path
            for (i = 7; i < strlen(urlofthispage); i++)
              if (urlofthispage[i] == '/')
                break;
            strcpy(result, urlofthispage);
            result[i] = 0;
            strncat(result, p1, (p2 - p1));
            return (int)(p2 - html + 1);       
          } else {
            //! the URL is a absolute path.
            len = strlen(urlofthispage);
    for (i = (len - 1); i >= 0; i--)
              if (urlofthispage[i] == '/')
                break;
            for (j = (len - 1); j >= 0; j--)
              if (urlofthispage[j] == '.')
              break;
            if (i == (len -1)) {
              //! urlofthis page is like http://www.abc.com/
                strcpy(result, urlofthispage);
                    result[i + 1] = 0;
                strncat(result, p1, p2 - p1);
                return (int)(p2 - html + 1);
            }
            if ((i <= 6)||(i > j)) {
              //! urlofthis page is like http://www.abc.com/~xyz
              //! or http://www.abc.com
              strcpy(result, urlofthispage);
              result[len] = '/';
              strncat(result, p1, p2 - p1);
              return (int)(p2 - html + 1);
            }
            strcpy(result, urlofthispage);
            result[i + 1] = 0;
            strncat(result, p1, p2 - p1);
            return (int)(p2 - html + 1);
          }
        }
      }   
  return -1;
}

long long int getsize()
{
    struct stat st; //variable which will count length of file.
    stat("/home/lakshay/haha/temp.txt",&st); // temp.txt is the file where wget fetch the html
    long long int file_size=st.st_size;
return file_size;
}




//first get a new url by getnexturl func
//then check if(this url already exists in the array
//after array is ready hash all
//function used -> int GetNextURL(char* html, char* urlofthispage, char* result, int pos)
void extractpages(FILE* fp1)
{
    //get url and depth
    char *a=malloc(URL_LENGTH+1),n;
	//memset(a,0,URL_LENGTH + 1);
    a=getcURL(fp1);
    char *d=malloc(2);
	//memset(d,0,2);
    d[0]=getdepth(fp1);
    d[1]='\0';
   long long int size=getsize();

    char* html=malloc(size+500);
	//memset(html,0,(size+1));
    size=0;
   
    do
        {
        n=fgetc(fp1);
        html[size++]=n;
        }
    while(n!=EOF);
   
    html[size]='\0';
    char *all[MAX_URL_PER_PAGE];
    int pos=0,i=0;
   
    while(1)
        {
        if(i > MAX_URL_PER_PAGE-1)
            break;
        int flag=0,j;
        char turl[1000];
       
        pos=GetNextURL(html,a,turl,pos);
        if(pos==-1)
            break;
       
        for(j=0;j<i;j++)
        {
            if(strcmp(turl,all[j])==0 && (strlen(turl)==strlen(all[j])))
                {flag=1;
                break;}
        }
        j=0;
        if(flag==0&&check_page(turl))
        {
            all[i]=malloc(strlen(turl)+1);
			//memset(all,0,(strlen(turl)+1));
            while(turl[j]!='\0')
                {
                all[i][j]=turl[j];
                j++;
                }
            all[i][j]='\0';
            i++;
        }

               
        }

    int j;
    for(j=0;j<i;j++)
    {
    insertll(all[j],d);
    }
    //setvisited(a);
    free(a);
    free(d);
    free(*all);
    struct mainll* m=start;
    while(m!=NULL)
    {
    printf("%s\t%d\t%d\n",m->info->url,m->info->depth,m->info->vis);
    m=m->next;
    }
}

FILE* filetrans(FILE* fp1,char* url,char* depth)
{
    char a;
    static int pageno=0;
    char name[64];
    sprintf(name, "/home/lakshay/haha/file%d.txt", pageno);
    pageno++;
    FILE* fp2=fopen(name,"w");
    char st[URL_LENGTH+300]={0};
    strcat(st, "NAME=");
    strcat(st, url);
    strcat(st, "\n");
    strcat(st, "DEPTH=");
    strcat(st, depth);
    strcat(st, "\n");
    fputs(st,fp2);
    do
        {
        a=fgetc(fp1);
        fputc(a,fp2);
        }
    while(a!=EOF);
    fclose(fp1);
    fclose(fp2);
    fp2=fopen(name,"r");
    return fp2;
}

int main(int argc, char *argv[])
{
int i;
if(checkDEPTH(argv[2])&&checkURL(argv[1])&&checkPARA(argc))
{
testDir(argv[3]);
insertll(argv[1],argv[2]);

while(1)
{
    int flag=0;
    struct mainll* m=start;
    while(m!=NULL)
        {
        if(m->info->vis==0)
            {flag=1;           
            break;}
        m=m->next;
        }
    if(flag==0) 
        break;
    getpage(m->info->url);
    char* dep=malloc(2);
	//memset(dep,0,2);
    dep[0]=m->info->depth+48;
    dep[1]='\0';
    FILE *fp1= fopen("/home/lakshay/haha/temp.txt","r");
    FILE *fp2= filetrans(fp1,m->info->url,dep);
    m->info->vis=1;
    extractpages(fp2);
}


}
else
printf("\nCheck Arguments\n");


return 0;
}

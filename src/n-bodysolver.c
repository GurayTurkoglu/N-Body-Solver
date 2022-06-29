#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"
#include <math.h>
#include <string.h>
#include <float.h>
#include <time.h>


void fileparser(int* id, double* mass, double* radius, double* posx, double* posy, double* init_vel_x, double* init_vel_y, int*maneuever_object, int* maneuever_time, double* maneuever_vel_x, double*maneuever_vel_y) 
{
	int filesize;
	int filesize_man;
	FILE *imported_file;
	FILE *imported_file_man;
	char fileinput[50];
	char man_fileinput[50];
	char *readedinput;
	char *readedinput_man;
	int i =0;
	int i2=0;
	char*parsed_text;
	char*maneuever_parse;
	double test;

	printf("\nEnter File Name: ");
	scanf("%s",fileinput);
	imported_file = fopen(fileinput,"r");
	fseek(imported_file,SEEK_SET,SEEK_END);
	filesize = ftell(imported_file);
	readedinput = (char *) calloc(filesize,sizeof(char));
	fclose(imported_file);
	imported_file = fopen(fileinput,"r");
	while(feof(imported_file) == NULL)
	{
		readedinput[i] = fgetc(imported_file);
		i = i++;
	}
	i=0;
	i2=0;
	printf("\nEnter Maneuever File Name: ");
	scanf("%s",man_fileinput);
	imported_file_man = fopen(man_fileinput,"r");
	fseek(imported_file_man,SEEK_SET,SEEK_END);
	filesize_man = ftell(imported_file_man);
	readedinput_man = (char *) calloc(filesize_man,sizeof(char));
	fclose(imported_file_man);
	imported_file_man = fopen(man_fileinput,"r");

	while(feof(imported_file_man) == NULL)
	{
		readedinput_man[i] = fgetc(imported_file_man);
		i = i++;
	}
	i=0;
	i2=0;
	fclose(imported_file);
	fclose(imported_file_man);
	maneuever_parse = strtok(readedinput_man,"#/");
	while(maneuever_parse!=NULL)
	{
		if (strstr(maneuever_parse,";")!=NULL)
		{
			test = strtod(maneuever_parse,NULL);
			printf("\nSAYI%d-%d:%lf",i,i2,test);
			maneuever_vel_y[i] = test;
			printf("\n\n");
			i=i++;
			i2=0;
		}
		else
		{
			test = strtod(maneuever_parse,NULL);
			switch (i2)
			{
				case 0:
					maneuever_object[i] = atoi(maneuever_parse);
				    break;
				case 1:
					maneuever_time[i] = atoi(maneuever_parse);
					break;
				case 2:
					maneuever_vel_x[i] = test;
					break;
			}	
			printf("\nSAYI%d-%d:%d",i,i2,test);
			i2++;
		}
		maneuever_parse = strtok(NULL,"#/");	
	}
	i =0;
	i2=0;
	parsed_text = strtok(readedinput,"#/");
	while(parsed_text!=NULL)
	{
		if (strstr(parsed_text,";")!=NULL)
		{
			test = strtod(parsed_text,NULL);
			//printf("\nSAYI%d-%d:%lf",i,i2,test);
			init_vel_y[i] = test;
			printf("\n\n");
			i=i++;
			i2=0;
		}
		else
		{
			test = strtod(parsed_text,NULL);
			printf("\nSAYI%d-%d:%lf",i,i2,test);
			id[i] = i;
			switch (i2)
			{
				case 0:
					mass[i] = test;
				    break;
				case 1:
					radius[i] = test;
					break;
				case 2:
					posx[i] = test;
					break;
				case 3:
					posy[i] = test;
					break;
				case 4:
					init_vel_x[i] = test;
					break;
			}
			i2++;

		}
		parsed_text = strtok(NULL,"#/");	
	}
	free(readedinput);
	free(parsed_text);
	free(maneuever_parse);
}

int main(int argc, char* argv[])
{
	FILE *logfile; //kayit dosyasi
	double **force_vector; 
	double *acc_vector_x; //x ivme vektoru
	double *acc_vector_y; //y ivme vektoru
	double *vel_vector_x; //x hiz vektoru
	double *vel_vector_y; //y hiz vektoru
	double **anglesat;
	int intx=0;
	int mancount=0;
//	int sign; //isaret
	int time_limit; //zaman limiti 
	double delta_time;
	double maneuever_vel_x=0;
	double maneuever_vel_y=0;
	int maneuever_time;
	int time; //zaman
	int angcounter=0; 
	int objcounter=0;
	int prevobj=0;
	int secim;
	//double range;
	//char buffer[256]; ILERIKI UPDATELER ICIN
	double logging_posx; //kayit icin pozisyonun x degeri 
	double logging_posy; //kayit icin pozisyonun y degeri
	double sum_x = 0; //x vektorlerinin toplami
	double sum_y = 0; //y vektorlerinin toplami
	int n;
	int maneuever_n;
	struct maneuever *maneuevers;
	struct maneuever
	{
		int *maneuever_object;
		int *maneuever_time;
		double *maneuever_vel_x;
		double *maneuever_vel_y;
	};

	struct objects *objectsptr;
	struct objects //OBJELER ICIN STURCT 
	{
		int *id;
		double *mass;
		double *positionx;
		double *positiony;
		double *init_veL_x;
		double *init_veL_y;
		double *radius;
		//int *isDestoyed;
	};

//	double angx; 
	printf("GRAV_CONST:\t"MAX_HASSAS,GRAV_SABIT); //definitions.h icerisinde bulunan yercekimi sabitini yazdirir.
	printf("\nEnter N number: "); //obje sayisi girisi.
	scanf_s("%d",&n,1);
	printf("\nEnter Maneuever number: "); //obje sayisi girisi.
	scanf_s("%d",&maneuever_n,1);
	printf("\nEnter Time Limit: "); //zaman limiti girisi.
	scanf_s("%d",&time_limit,1);
	printf("\nEnter Delta Time: "); //zaman limiti girisi.
	scanf_s("%lf",&delta_time,1);
	maneuevers = (struct maneuever*) malloc(maneuever_n*sizeof(struct maneuever)); //obje pointerini hafizada malloc eder.
	objectsptr = (struct objects *) malloc(n*sizeof(struct objects)); //obje pointerini hafizada malloc eder.
	acc_vector_x = (double *) calloc(n,sizeof(double)); //x ivme vektoru pointerini hafizada alloke eder.
	acc_vector_y = (double *) calloc(n,sizeof(double)); //y ivme vektoru pointerini hafizada alloke eder.
	vel_vector_x = (double *) calloc(n,sizeof(double)); //x hiz vektoru pointerini hafizada alloke eder ve calloc ile 0 degeri atar.
	vel_vector_y = (double *) calloc(n,sizeof(double)); //y hiz vektoru pointerini hafizada alloke eder ve calloc ile 0 degeri atar.
	
	maneuevers->maneuever_object = (int *) malloc(maneuever_n*sizeof(int)); //obje idsinin oldugu pointeri hafizada alloke eder.
	maneuevers->maneuever_time = (int *) malloc(maneuever_n*sizeof(int));
	maneuevers->maneuever_vel_x = (double *) malloc(maneuever_n*sizeof(double)); //obje yaricapi hafiza allokasyonu.
	maneuevers->maneuever_vel_y = (double *) malloc(maneuever_n*sizeof(double)); //obje yaricapi hafiza allokasyonu.

	objectsptr->id = (int *) malloc(n*sizeof(int)); //obje idsinin oldugu pointeri hafizada alloke eder.
	objectsptr->mass = (double *) malloc(n*sizeof(double)); //obje kutlesinin pointerini hafizada alloke eder.
	objectsptr->positionx = (double *) malloc(n*sizeof(double)); //x pozisyonu hafiza allokasyonu.
	objectsptr->radius = (double *) malloc(n*sizeof(double)); //obje yaricapi hafiza allokasyonu.
    objectsptr->positiony = (double *) malloc(n*sizeof(double)); //y pozisyonu hafiza allokasyonu.
	objectsptr->init_veL_x = (double *) malloc(n*sizeof(double)); //initial(baslangic) kosullarindaki x ivme degeri.
	objectsptr->init_veL_y = (double *) malloc(n*sizeof(double)); //initial(baslangic) kosullarindaki y ivme degeri.
	//objectsptr->isDestoyed = (int *) malloc(n*sizeof(int)); //initial(baslangic) kosullarindaki y ivme degeri.

	force_vector = malloc(n*sizeof(double *)); //kuvvet vektoru hafiza allokasyonu
	anglesat = malloc(n*sizeof(double *)); //objeler arasi aci degeri hafiza allokasyonu

	for(intx = 0;intx<n;intx++) 
	{
	 anglesat[intx] = malloc((n-1)*sizeof(double));
	}
	for(intx = 0;intx<n;intx++)
	{
	 force_vector[intx] = malloc((n-1)*sizeof(double));
	}

	intx =0;

	tekrarsec:
	printf("\nSELECTIONS: \n \t 1-) Manual Enter \n \t 2-) File Parser \n"); //secim.
	printf("Selection: ");
	scanf("%d",&secim);
	switch(secim)
	{
		case 1:
			goto devam;
			break;
		case 2:
			goto devam;
			break;
	default:
		system("cls");
		goto tekrarsec;
		break;
	}
devam:
if (secim == 1)
{
	//*********************************************MANUEL GIRDI****************************************************/
	for(intx=0;intx<n;intx++)//HER BIR OBJENIN PARAMETRELERINI ALARAK OLUSTURUR
	{
		objectsptr->id[intx] = intx;	
		printf("Enter Mass Value(kg): \n"); 
		scanf_s("%lf",&objectsptr->mass[intx],1);	 //struct icerisinde bulunan mass[id] icin kullanici girisini atar. 
		printf("Enter Radius Value(m): \n"); 
		scanf_s("%lf",&objectsptr->radius[intx],1); //struct icerisinde bulunan radius[id] icin kullanici girisini atar. 
		printf("Enter Position X Value: \n"); 
		scanf_s("%lf",&objectsptr->positionx[intx],1); //struct icerisinde bulunan x pozisyonlarının oldugu positionx[intx] icin kullanici girisini atar. 
		printf("Enter Position Y Value: \n"); 
		scanf_s("%lf",&objectsptr->positiony[intx],1);  //struct icerisinde bulunan y pozisyonlarının oldugu positiony[intx] icin kullanici girisini atar. 
		printf("Enter Initial X Acceleration Value(m/s): \n"); 
		scanf_s("%lf",&objectsptr->init_veL_x[intx],1); //initial(baslangic) ivme x degeri girisini atar
		printf("Enter Initial Y Acceleration Value(m/s): \n"); 
		scanf_s("%lf",&objectsptr->init_veL_y[intx],1);  //initial(baslangic) ivme y degeri girisini atar
	}
	//*************************************************************************************************************/
}	
if (secim==2)
{
	fileparser(objectsptr->id,objectsptr->mass, objectsptr->radius,objectsptr->positionx,objectsptr->positiony,objectsptr->init_veL_x,objectsptr->init_veL_y,maneuevers->maneuever_object,maneuevers->maneuever_time,maneuevers->maneuever_vel_x,maneuevers->maneuever_vel_y); //GELECEK UPDATELERDE KULLANILACAKTIR.
}

//for(intx=0;intx<n;intx++)
//{
	//objectsptr->isDestoyed[intx] == 0;
//}

logfile = fopen("logging.txt","a");
for (time = 1;time<time_limit;time=time+delta_time)
{
	objcounter=0;
	///*********************HER BIR OBJENIN DEGERLERINI YAZDIRIR (GEREKSIZ BIRCOK PRINTF YAPTIGI ICIN KULLANILMAMAKTADIR).************ 
	//printf("\n\n\n\n**************************************************");
   // printf("\n--------------------------------------------------");
	//printf("\n----------------N_BODY_CALCULATOR-----------------");
	//printf("\n--------------------------------------------------");
	//printf("\n**************************************************\n\n\n\n");

	//for(intx=0;intx<n;intx++)
	//{
	  //printf("\n--------------------------------------------------");
	  //printf("\nN-BODY:");
	  //printf("|"WRITE_INTEGER"|\n\n",objectsptr->id[intx]);
	 // printf("MASS: "DUSUK_HASSAS"\t",objectsptr->mass[intx]);
	  //printf("POS X: "DUSUK_HASSAS"\t",objectsptr->positionx[intx]);
	 // printf("POS Y: "DUSUK_HASSAS"\t",objectsptr->positiony[intx]);
	  //printf("\n--------------------------------------------------");
	//}
	///********************************************************************************************************************************

	for(intx=n-1;intx>=0;intx--)
	{
	   angcounter = 0;

	   //************************HANGI OBJE ICIN N-BODY ISLEMI YAPILDIGINI YAZDIRIR**************************
	   //printf("\n\nCALCULATIONG FOR OBJECT: "WRITE_INTEGER"\n",objectsptr->id[intx]);
	   //****************************************************************************************************

	   for(prevobj=n-1;prevobj>=0;prevobj--) //ANLIK SECILI OBJE ILE KALAN BUTUN OBJELER ARASINDAKI KUVVET VEKTORLERI HESAPLANIR
	   {
		 if(prevobj!=intx)//EGER OBJE KENDISI DEGIL ISE;
		 {
		  //printf("\nEFF_OBJ : |"WRITE_INTEGER"|",objectsptr->id[prevobj]);// su anki islemden EFF_OBJ(EFFECTED OBJECT) (ETKILENEN OBJEYI) yazdirir	  
		  anglesat[objcounter][angcounter] = (atan2((objectsptr->positiony[intx]-objectsptr->positiony[prevobj]),(objectsptr->positionx[intx] - objectsptr->positionx[prevobj])));//*(180.0/M_PI))+360.0; //arctanjant y2-y1/x2-x1 formulunu kullanarak iki obje arasi aciyi bulur ve degree(derece) cinsine cevirir.
		  //anglesat[objcounter][angcounter] = fmod(anglesat[objcounter][angcounter],360.0);
		  force_vector[objcounter][angcounter] = (GRAV_SABIT*(objectsptr->mass[intx])*(objectsptr->mass[prevobj]))/(pow(sqrt(pow(objectsptr->positiony[intx]-objectsptr->positiony[prevobj],2)+pow(objectsptr->positionx[intx] - objectsptr->positionx[prevobj],2)),2)); //definitions.h icerisindeki GRAV_SABIT(YERCEKIMI SABITI) DEGERI KULLANILARAK KUVVET HESAPLAMALARI SIN VE COS BILESENLERI ILE YAPILIR.

		 /* range = sqrt(pow(fabs(objectsptr->positiony[intx]-objectsptr->positiony[prevobj]),2)+pow(fabs(objectsptr->positionx[intx]-objectsptr->positionx[prevobj]),2));	

		  if(range < objectsptr->radius[intx]+objectsptr->radius[prevobj])
		  {
			  if(objectsptr->mass[intx] > objectsptr->mass[prevobj])
			  {
			   objectsptr->isDestoyed[prevobj] = 1;
			   objectsptr->mass[prevobj] = 0;
			  }
	      }
		  */
		  angcounter=angcounter++;
		 }
	   }
	   objcounter = objcounter++;
	}
	for(objcounter=0;objcounter<n;objcounter++)
	{
	  sum_x=0;
	  sum_y=0;
	  for(angcounter=0;angcounter<n-1;angcounter++)
	  {
	  // printf("\n\n--------------------------------------------------");
	  // printf("\nANG %d, %d: "MAX_HASSAS"\t",objcounter,angcounter,anglesat[objcounter][angcounter]);
	 //  printf("\nF_VECTOR %d, %d: "MAX_HASSAS"\t",objcounter,angcounter,force_vector[objcounter][angcounter]);
      // printf("\nCOSINE : %.5lf\n",cos((anglesat[objcounter][angcounter])*(M_PI/180.0)));
	   //printf("SINE : %.5lf\n",sin((anglesat[objcounter][angcounter])*(M_PI/180.0)));
	   sum_x = sum_x+(force_vector[objcounter][angcounter]*cos((anglesat[objcounter][angcounter])));//*(M_PI/180.0)));
	   sum_y = sum_y+(force_vector[objcounter][angcounter]*sin((anglesat[objcounter][angcounter])));//*(M_PI/180.0)));
	  // printf("\n--------------------------------------------------");
	  }
	  
	  for(mancount=0;mancount<maneuever_n;mancount++)
	  {
		  if (maneuevers->maneuever_object[mancount] == objcounter)
		  {
			  maneuever_time = maneuevers->maneuever_time[mancount];
			  if (time >= maneuever_time)
			  {
				maneuever_vel_x = maneuevers->maneuever_vel_x[mancount];
				maneuever_vel_y = maneuevers->maneuever_vel_y[mancount];
			  }
		  }
	  }

	  acc_vector_x[objcounter] = (sum_x/(objectsptr->mass[(n-1)-objcounter]))*delta_time;
	  acc_vector_y[objcounter] = (sum_y/(objectsptr->mass[(n-1)-objcounter]))*delta_time;
	  vel_vector_x[objcounter] = vel_vector_x[objcounter]+acc_vector_x[objcounter];
	  vel_vector_y[objcounter] = vel_vector_y[objcounter]+acc_vector_y[objcounter];
	 // printf("\nFORCESUM_X = " MAX_HASSAS"\t",sum_x);
	 // printf("\nFORCESUM_Y = " MAX_HASSAS"\t",sum_y);
	 // printf("\nACC_X = " MAX_HASSAS"\t",acc_vector_x[objcounter]);
	 // printf("\nACC_Y = " MAX_HASSAS"\t",acc_vector_y[objcounter]);
	  
	 // printf("\nPOX : %.5lf\n",(*objectsptr).positionx[(n-1)-objcounter]);
	 // printf("\nPOY : %.5lf\n",(*objectsptr).positiony[(n-1)-objcounter]);
	  (*objectsptr).positionx[(n-1)-objcounter] = (*objectsptr).positionx[(n-1)-objcounter]+(objectsptr->init_veL_x[(n-1)-objcounter]*delta_time)+(maneuever_vel_x*delta_time)-(vel_vector_x[objcounter]*delta_time); //BIRIM ZAMANDAKI POZISYON DEGISIMI X EKSENINDE HESAPLANIR.
	  (*objectsptr).positiony[(n-1)-objcounter] = (*objectsptr).positiony[(n-1)-objcounter]+(objectsptr->init_veL_y[(n-1)-objcounter]*delta_time)+(maneuever_vel_y*delta_time)-(vel_vector_y[objcounter]*delta_time); //BIRIM ZAMANDAKI POZISYON DEGISIMI Y EKSENINDE HESAPLANIR.
	  logging_posx =  (*objectsptr).positionx[(n-1)-objcounter]; //x pozisyonu kayit eder.
	  logging_posy =  (*objectsptr).positiony[(n-1)-objcounter]; //y pozisyonu kayit eder.
	 // printf("\nPosX : %lf PosY : %lf ",logging_posx,logging_posy);
	
	  //if (objectsptr->isDestoyed[objcounter] != 1)
	  //{
	  fprintf(logfile,"%d \t %lf \t %lf\n",objcounter,logging_posx,logging_posy);
	  //}
	}
}
	fclose(logfile);
    printf("\nCalculation Done. Press A Key To Exit."); //HESAPLAMA SONU UYARISI.
	///************BUTUN HAFIZADA ALLOKE EDILMIS OBJELERI FREE EDER*********************
	free(acc_vector_x);
	free(acc_vector_y);
	free(vel_vector_x);
	free(vel_vector_y);
	free(force_vector);
	free(anglesat);
	free(objectsptr->id);
	free(objectsptr->init_veL_x);
	free(objectsptr->init_veL_y);
	free(objectsptr->mass);
	free(objectsptr->positionx);
	free(objectsptr->positiony);
	free(objectsptr->radius);
	free(objectsptr);
	free(maneuevers->maneuever_object);
	free(maneuevers->maneuever_time);
	free(maneuevers->maneuever_vel_x);
	free(maneuevers->maneuever_vel_y);
	free(maneuevers);
	//***********************************************************************************
	exit(0);

}

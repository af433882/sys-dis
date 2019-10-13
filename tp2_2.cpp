#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define TAILLE_PS 4
#define TAILLE_PT 4

int main()
{
    //soit un processus pere Pi creant 2 processus : ps et pt
    pid_t pid_ps;
    pid_t pid_pt;

    //init des partitions pt et ps

    int part_ps[TAILLE_PS]={15,7,3,20};
    int part_pt[TAILLE_PT]={10,5,21,2};

    int tube_pt[2];
    int tube_ps[2];
    pipe(tube_pt);
    pipe(tube_ps);

    int min_pt=99999999; //min
	int min_pt_pos=-1;//indice_min
    int max_ps=0; //max 
	int max_ps_pos=-1;//indice_max

    //creation du 1er fils
    pid_ps = fork();

    if(pid_ps == 0)
    {
        /* Code du processus PS */
        close(tube_pt[1]);
		close(tube_ps[0]);

        while(1){
		    
		    //recherche du minimum
		    for(int j=0; j<TAILLE_PS; j++)
		    {
		       if(part_ps[j]>=max_ps)
		       {
		       		max_ps=part_ps[j];
		            max_ps_pos=j;
		       }
			}
            //envoie
            //int* p =&max_ps;
            printf(" PS envoie  %d \n", max_ps);
		    write(tube_ps[1],&max_ps,sizeof(int));
            
            //reception
            sleep(2);
            int recu;
		    double l =read(tube_pt[0], &recu, sizeof(int));
            printf(" Reception valeur l %d par PS \n", l);
            printf(" Reception valeur %d par PS \n", recu);
            //la valeur est mise dans part_ps
		    part_ps[max_ps_pos]=recu;
			
            //Affichage de part PS:
            printf("PS: %d, %d, %d, %d \n",part_ps[0],part_ps[1],part_ps[2],part_ps[3]);  

        }     
    }

    else
    {
        pid_pt=fork();

        if(pid_pt== 0)
        {
            /* Code du processus PT */
            close(tube_pt[0]);
			close(tube_ps[1]);

            while (1)
            {
                //recherche du minimum
				for(int j=0; j<TAILLE_PT; j++)
				{
				   if(part_pt[j]<=min_pt)
				   {
				   		min_pt=part_pt[j];
				        min_pt_pos=j;
				   }
				}
                //envoie
                //int* m =&min_pt;
				printf(" PT envoie  %d \n", min_pt);
				write(tube_pt[1],&min_pt,sizeof(int));
                //reception 
				sleep(2);
				int recu2;
				double l =read(tube_ps[0], &recu2, sizeof(int));
                printf(" Reception valeur l %d par PT \n", l);
                printf(" Reception valeur %d par PT \n", recu2);
				//la valeur est mise dans part_ps
				part_pt[min_pt_pos]=recu2;
                
                //Affichage de part PT:
				printf("PT: %d, %d, %d, %d \n",part_pt[0],part_pt[1],part_pt[2],part_pt[3]);    
            }
            
        }
    }
    




    int a =1;
    wait(&a);
	wait(&a);
    return 0;
}
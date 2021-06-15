### **Running Eigenface Program**

> `srun eigenfaces_seq.exe -i /gscratch/niac/amath583sp21/data/celebA/celebA_gray_lowres.262781_55_45_32.bin -n 500 -f 8`

---
### **Load Modules from Hyak**

> `module load gcc/10.2.0`

> `module load cuda/11.2.2`

> `module load intel/oneAPI`

> `module load intel/oneAPI && module load cuda/11.2.2 && module load gcc/10.2.0`



---
### **Sync to/from Hyak:** 

> `rsync -avuzb ps8 hongdali@klone.hyak.uw.edu:` Sync to

> `rsync -avuzb hongdali@klone.hyak.uw.edu:ps8 ./` Sync from remote, making a new empty directory for it will be better. 

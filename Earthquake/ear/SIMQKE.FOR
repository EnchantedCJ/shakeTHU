!     根据MIT的SIMQKE程序稍加修改计算人工地震波
!     已经默认了若干参数，只需修改地震波加强和衰弱时间以考虑地震波的持时差异
!     By LuXZ@Tsinghua
!
	program Main

	open(55,file="DataOut.Txt")
		read(55,*) xLuXZT1, xLuXZT2, xLuXZT3 , Tg
	close(55)
	

	call BuildSpecturm(Tg) ! 生成目标反应谱	
	call SIMQKE(xLuXZT1,xLuXZT2, xLuXZT3) ! 生成地震动
	call GetMAG() ! 计算实际反应谱

	stop
	end program

      Subroutine SIMQKE(xLuXZT1,xLuXZT2, xLuXZT3)
C                                                                       
C     - SIMULATION OF EARTHQUAKE GROUND MOTIONS -                       
C                                                                       
C     DEVELOPED BY - E. H. VANMARCKE, C. A. CORNELL,                    
C                    D. A. GASPARINI AND S. N. HOU                      
C                    DEPARTMENT OF CIVIL ENGINEERING                    
C                    MASSACHUSETTS INSTITUTE OF TECHNOLOGY              
C                    CAMBRIDGE, MASSACHUSETTS 02139                     
C                                                                       
C     PROGRAM DATE - AUGUST 1969, REVISED SEPTEMBER 1976                
C                                                                       
C     NOTES - DUMMY SUBROUTINE PLOT CALLS (SC4020) HAVE BEEN INSERTED   
C
C     MODIFIED TO RUN ON IBM-PC NOVEMBER 1988 By Thomas F. Blake
C                   - USES FREE FORMAT FOR DATA INPUT FILE
C                   - MODIFIED SOME OUTPUT FORMATS
C                   - ADDED SCREEN- AND PRINTOUT-HEADINGS
C                   - MODIFIED RANDOM NUMBER GENERATOR TO WORK WITH
C                       THE LIMITED INTEGER CAPACITY OF IBM-PC
C
C     DISTRIBUTED BY NISEE / COMPUTER APPLICATIONS
C                                                                       
C INPUT PARAMETERS REQUIRED                                             
C                                                                       
C  IX--A STARTER FOR THE RANDOM NUMBER GENERATOR-IT MUST BE ODD         
C  NPA---NUMBER OF DIFFERENT MOTIONS REQUIRED                           
C  ICASE---=1 FOR STATIONARY CASE                                       
C  TL - THE LARGEST PERIOD VALUE FOR RESPONSE CALCULATIONS              
C  TS - THE SMALLEST VALUE                                              
C  TMIN,TMAX---OPTIONAL MINIMUM AND MAXIMUM PERIODS TO DETERMINE FREQUENCY
C  CONTENT OF THE MOTION. DEFAULT USES TS AND TL                        
C  NCYCLE---THE NUMBER OF ITERATIONS TO BE PERFORMED IS ONE LESS        
C  THAN THIS NUMBER--IF NCYCLE = 1, NO ITERATION IS MADE                
C  DELT -- TIME INTERVAL USED BETWEEN POINTS                            
C  NDAMP---NUMBER OF DIFFERENT DAMPINGS TO BE CONSIDERED                
C  AMOR---ARRAY CONTAINING THE DAMPING VALUES                           
C  TRISE --- RISE TIME                                                  
C  TLVL --- INTERVAL AT THE HIGHEST AMPLITUDE                           
C  NGWK -- DEFINES TYPE OF SPECTRAL DENSITY FUNCTION USED               
C  IF NGWK = 0 , THE PROGRAM GENERATES ITS OWN POWER SPECTRUM.          
C  IF NGWK IS NOT = 0, THEN A PIECEWISE LINEAR POWER SPECTRUM           
C  WILL BE PROVIDED BY USER AND NGWK = NUMBER OF POINTS THAT DEFINE IT. 
C  IF NGWK IS NEGATIVE, THEN GWK WILL BE READ ALONG WITH PERIODS FOR    
C  RESPONSE CALCULATIONS                                                
C  ABS(NKK) = NUMBER OF POINTS FOR RESPONSE CALCULATIONS.               
C  IF NKK IS POSITIVE, THE PROGRAM WILL GENERATE A STRING OF POINTS     
C  ON A LOGARITHMIC SCALE FROM TS TO TL.                                
C  IF NKK IS NEGATIVE, THE USER PROVIDES A LIST OF POINTS.              
C  (TSV,SV0) - POINTS WHICH DEFINE DESIRED VELOCITY RESPONSE SPECTRUM   
C  NRES---NUMBER OF POINTS WHICH DEFINE DESIRED RESPONSE SPECTRUM       
C  IF NRES = 0, NO DATA NEED BE GIVEN(NO CYCLING ONLY).                 
C  (W0,GWK0) - POINTS THAT DEFINE POWER SPECTRUM IF NGWK IS NOT = 0.    
C  TQ---OPTIONAL ARRAY OF PERIOD VALUES FOR RSPONSE CALCULATIONS.       
C  AGMX --- MAX GROUND ACC  INPUT UNIT IN G                             
C  DUR --- DURATION                                                     
C  UNITS   SECONDS,INCHES---UNLESS SPECIFIED OTHERWISE                  
C                                                                       
C*CDC1              (INPUT,OUTPUT,TAPE5=INPUT,TAPE6=OUTPUT,PUNCH)       
      DIMENSION TQI(150)                                                
      DIMENSION RR(300)                                                 
      DIMENSION YTITL(9),TITLO(9)                                       
      DIMENSION TIT(9),TIM(9),TIMX(9),TIMY(9),TIX(9),TITX(9),TITY(9)    
      DIMENSION ACCG(8001),WB(300),GWK(300),TIME(3001),FRQ(300),        
     1          TQ(300),PLTVMX(10,300),AMOR(10),TITLE(20),IBUF(2000),   
     2          FQ(1500),GWG(1500),PA(1500),DW(1500),TMD(10,300),       
     3          W0(300),GWK0(300),SV(300),TSV(1010),SV0(1010),SI(300),   
     4          ANEWGK(300)                                            
      DIMENSION PERCEN(300)
      EQUIVALENCE(TIME(1),FQ(1)),(TIME(1501),DW(1)),(GWG(1),PLTVMX(1,1))
      CHARACTER*40 IFNAME,OFNAME
C     DATA TIX/'    ','    ','RESP','ONSE',' SPE','CTRU','M   ','    ',
C    1         '    '/
C     DATA TIM/'    ','    ','ACCE','LERO','GRAM','    ','    ','    ',
C    1         '    '/                                                
C     DATA BLANK /'    '/                                             
C     DATA TIT/'RESP','ONSE',' SPE','CTRU','M  D','AMPI','NG  ','    ',
C    1         '    '/
C     DATA TITX/'    ','  NA','TURA','L PE','RIOD','    ','(SEC','ONDS',
C    1         ')   '/
C     DATA YTITL/'    ','G(W)',' - (','IN**','2/SE','C**3',')   ',
C    1           '    ','    '/
C     DATA TITLO/'SPEC','TRAL',' DEN','SITY',' FUN','CTIO','N   ',
C    1           '    ','    '/
C     DATA TITY/'    ',''    ','MAXI','MUM ','VELO','CITY',' (IN',
C    1           '|SEC',')   '/
C     DATA TIMX/'    ','    ','TIME',' (SE','COND','S)  ','    ','    ',
C    1          '    '/
C     DATA TIMY/'    ','ACCE','LERA','TION','    ',' G s','    ','    ',
C    1          '    '/                                                
      BETAS=0.005
      BETAL=0.2 
      PI=3.14159                           
      ICONT=0
C
      WRITE(*,9001)
 9001 FORMAT(///////19X,'*********************************************'/
     +              19X,'*                                           *'/
     +              19X,'*            * * S I M Q K E * *            *'/
     +              19X,'*                                           *'/
     +              19X,'*           - IBM-PC ADAPTATION -           *'/
     +              19X,'*                                           *'/
     +              19X,'*          A PROGRAM FOR ARTIFICIAL         *'/
     +              19X,'*             MOTION GENERATION             *'/
     +              19X,'*                                           *'/
     +              19X,'*  Original Program: Vanmarke et al. (1976) *'/
     +              19X,'*                                           *'/
     +              19X,'*   IBM-PC Version: Thomas F. Blake (1988)  *'/
     +              19X,'* Distributed: NISEE/Computer Applications  *'/
     +              19X,'*                                           *'/
     +              19X,'*********************************************'/
     +              ///)
C
C    SPECIFY INPUT AND OUTPUT FILE NAMES & OPEN FILES
C
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
c      WRITE(*,'(A\)') ' ENTER NAME OF INPUT FILE - '
c      READ(*,'(A)') IFNAME
	IFNAME="luxzin.txt"
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      OPEN(5,FILE=IFNAME,STATUS='OLD')
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
c      WRITE (*,'(A)') '  '
c      WRITE (*,'(A\)') ' ENTER NAME OF OUTPUT FILE - '
c      READ(*,'(A)') OFNAME
	OFNAME="luxzout.txt"
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      OPEN(6,FILE=OFNAME)
C
C    WRITE HEADING TO OUTPUT FILE
C
      WRITE (6,9001)
C                                                                       
C    REQUIRED INPUT PARAMETERS                                          
C                                                                       
 9003 READ (5,1) TITLE                                                  
C     CALL STOIDV ('M5324-9950',9,0)                                    
      READ (5,*) TS,TL,TMIN1,TMAX1,YMIN,YMAX                         
      READ (5,*) ICASE,TRISE,TLVL,DUR,AO,ALFAO,BETAO,IPOW            
      READ (5,*) DELT,AGMX,IIX,NDAMP,NCYCLE,NPA,NKK,NRES,NGWK,IPCH    
      AGMX=AGMX*32.2*12.     
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
c	自动生成随机数
	CALL RANDOM_SEED()
	CALL RANDOM_NUMBER(x)
	IIX=x*1000.
	TRISE=xLuXZT1;
	TLVL=xLuXZT2;
	DUR=xLuXZT3;
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!                                           
C                                                                       
C    FIRST DAMPING VALUE MUST BE ONE WHICH IS CYCLED ON.                
C    THE FIRST CURVE VALUE WILL BE PLOTTED (RESPONSE SPECTRUM)          
C                                                                       
      READ(5,*) (AMOR(I),I=1,NDAMP)                                  
      WRITE (6,2) TITLE                                                 
      WRITE(6,30) DELT                                                  
C                                                                       
      IF (NKK.LE.0) GO TO 6301                                          
C                                                                       
C    OPTIONS 1 AND 2                                                    
      CALL PLTX2(TS,TL,TQ,NKK)                                          
      GO TO 3                                                           
C                                                                       
C    OPTION 3                                                           
 6301 NKK=-NKK                                                          
C                                                                       
C    OPTIONAL INPUT PARAMETERS IF NKK IS NEGATIVE.                      
C    GWK IS REQUIRED ONLY IF NGWK IS NEGATIVE.                          
C                                                                       
      READ (5,*) (TQ(I),I=1,NKK)                                       
      READ (5,*)(GWK(NKK-I+1),I=1,NKK)                                
      READ (5,*) N2,N3                                               
   14 READ (5,*) TC,GWC                                              
      IF (TC.GT.50.0) GO TO 5                                           
      DO 9 I=1,NKK                                                      
      IF (ABS(TC-TQ(I)).LT.0.0002) GO TO 11                             
    9 CONTINUE                                                          
      GO TO 14                                                          
   11 GWK(NKK-I+1)=GWC                                                  
      GO TO 14                                                          
    5 CONTINUE                                                          
      IF (N2.EQ.0) GO TO 3                                              
      DO 10 I=1,N3                                                      
      READ (5,*) TQ1,TQ2,RATIO                                       
      DO 10 J=1,NKK                                                     
      IF (TQ(J).GT.TQ1.AND.TQ(J).LT.TQ2) GWK(NKK-J+1)=GWK(NKK-J+1)*RATIO
   10 CONTINUE                                                          
    3 DO 4325 I=1,NKK                                                   
      J=NKK-I+1                                                         
      FRQ(I)=1./TQ(I)                                                   
4325  WB(J)=6.2832/TQ(I)                                                
      IF (TMIN1.EQ.0.)  TMIN1=TS                                        
      WL=6.2832/TMIN1                                                   
      IF (TMAX1.EQ.0.)  TMAX1=TL                                        
      WS=6.2832/TMAX1                                                   
C                                                                       
C    WEND --- THE HIGHEST FREQUENCY FOR GROUND MOTION                   
C    WBEGIN --- THE LOWEST FREQUENCY FOR GROUND MOTION                  
C    THE FOLLOWING OPTIONS FOR COMPUTING WEND AND WBEGIN MAY BE         
C    ELIMINATED SINCE BETAL AND BETAS HAVE BEEN DEFINED INTERNALLY BY   
C    THE PROGRAM TO BE 0.2 AND 0.005 RESPECTIVELY                       
C                                                                       
      WEND=2.0*WL                                                       
      IF ((5.0*BETAL).GE.1.0) WEND=WL*(1.+5.*BETAL)                     
      WBEGIN=WS*.5                                                      
      IF (BETAL.LT.0.05)  WBEGIN=WS*(1.-10.*BETAL)                      
      IF(ICASE.GT.1) GO TO 42                                           
C                                                                       
C    NO INTENSITY ENVELOPE USED                                         
      WRITE(6,134)                                                      
      GO TO 38                                                          
   42 WRITE(6,135)                                                      
   38 WRITE(6,106)AGMX                                                  
      IF (NRES.EQ.0) GO TO 6022   

!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
	open(51,file='Site.txt')
      READ(51,*)(TSV(I),SV0(I),I=1,NRES)                                    
	close(51)
!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	CALL POLATE(NRES,NKK,TSV,SV0,TQ,SV)                               
      WRITE(6,107) TRISE,TLVL,DUR                                       
      WRITE(6,6016)                                                     
6022  IF (NGWK.EQ.0) GO TO 4260                                         
      IF(NGWK.LT.0) GO TO 9703                                          
C                                                                       
C    OPTIONAL INPUT OF ORIGINAL POWER SPECTRUM IF NGWK IS POSITIVE      
C    IF TQ WAS READ IN PREVIOUSY FOR NKK NEGATIVE, THIS OVERIDES POWER  
C    SPECTRUM 'GWK' READ IN WITH 'TQ'.                                  
C                                                                       
C                                                                       
C    OPTIONAL INPUT OF DESIRED RESPONSE VELOCITY                        
C    SPECTRUM IF CYCLING IS USED.                                       
C                                                                       
      READ (5,*) (W0(I),GWK0(I),I=1,NGWK)                            
      CALL POLATE(NGWK,NKK,W0,GWK0,WB,GWK)                              
9703  DO 8011 I=1,NKK                                                   
      J=NKK+1-I                                                         
      GWK0(I)=GWK(I)                                                    
 8011 WRITE(6,4340) TQ(I),FRQ(I),GWK(J)                                 
      GO TO 6007                                                        
 4260 T=(DUR+TLVL)/2.                                                   
      BETA=AMOR(1)                                                      
      CALL SVGW(NKK,WB,GWK0,SV,T,BETA,16.0,0.6,0.368,GSUM,WCP,QP,RR)    
      DO 6001 LLL=1,NKK                                                 
      LL1=NKK-LLL+1                                                     
 6001 WRITE(6,8901)TQ(LL1),FRQ(LL1),GWK0(LLL),RR(LLL)                   
      WRITE (6,8902) WCP,QP                                             
C    SET THE MAXIMUM VALUE OF SPECTRAL DENSITY FUNCTION FOR PLOT        
      XMAX= 0.0                                                         
      DO 327 I12= 1,NKK                                                 
      IF (XMAX-GWK0(I12)) 326,327,327                                   
  326 XMAX=GWK0(I12)                                                    
  327 CONTINUE                                                          
      IF (XMAX-70.0) 329,328,328                                        
  328 XLAI=XMAX/100.                                                    
      NDUM=(IFIX(XLAI)+1)*100                                           
      XMAX=FLOAT(NDUM)                                                  
      GO TO 330                                                         
  329 XMAX=70.0                                                         
  330 CONTINUE                                                          
      CALL GWPLOT (NKK,0.01,4.0,0.0,XMAX,TQ,GWK0,TITX,TITLO,YTITL)      
      AREA=SQRT(GSUM)/386.4                                             
      WRITE(6,6008) AREA                                                
6007  ITOTAL=NDAMP*NKK                                                  
      IX=(IIX/2)*2+1                                                    
C                                                                       
C    LOOP OVER NPA, NUMBER OF ARTIFICIAL EARTHQUAKES DESIRED            
      DO 585 NTOTAL=1,NPA                                               
      WRITE(6,60) IX                                                    
      DO 8608 I=1,NKK                                                   
 8608 GWK(I)=GWK0(I)                                                    
      MM=1                                                              
      AREAG=0.                                                          
      SIGMS=0.                                                          
      NFQ=0                                                             
      W=WBEGIN                                                          
 4080 DELW=BETAS*W                                                      
      W=W+DELW                                                          
      CALL DUMMY(W,FOUT,NKK,WB,GWK,MM)                                  
      NFQ=NFQ+1                                                         
      GWG(NFQ)=FOUT                                                     
      FQ(NFQ)=W                                                         
      DW(NFQ)=DELW                                                      
      AREAG=AREAG+GWG(NFQ)*DELW                                         
      SIGMS=SIGMS+GWG(NFQ)*DELW*W*W                                     
      IF (W.LT.WEND) GO TO 4080                                         
C                                                                       
C    LOOP OVER NCYCLE, TO SMOOTHEN RESPONSE SPECTRUM FOR TARGET DAMPING 
      DO 100 ICYCLE=1,NCYCLE                                            
C                                                                       
C    W IS LOWEST FREQUENCY REPRESENTED IN GROUND MOTION.                
C                                                                       
      IF (ICYCLE.LE.1) GO TO 1116                                       
      AREAG=0.                                                          
      MM=1                                                              
      DO 6703 I=1,NFQ                                                   
      W=FQ(I)                                                           
      CALL DUMMX(W,FOUT,NKK,WB,GWK,MM)                                  
      GWG(I)=FOUT                                                       
 6703 AREAG=AREAG+DW(I)*GWG(I)                                          
 1116 DO 1117 IP=1,NFQ                                                  
 1117 GWG(IP)=GWG(IP)*DW(IP)*2.                                         
      IF(ICYCLE.GT.1) GO TO 8603                                        
C                                                                       
C    COMPUTE AVERAGE FREQUENCY AND PERIOD                               
C                                                                       
      SIGMS=SIGMS/AREAG                                                 
      WA=SQRT(SIGMS)                                                    
      TA=6.2832/WA                                                      
C                                                                       
C    DEFINE SLOPES OF ENVELOPE                                          
C                                                                       
      IF (ICASE.GT.2) GO TO 6                                           
      IF(TRISE.GT..0) GO TO 33                                          
      TRISE=0.25*DUR                                                    
      TLVL=0.                                                           
   33 IF(ICASE.LE.1) GO TO 7                                            
    8 FTC1=1./TRISE                                                     
      FTC2=-1./(DUR-TRISE-TLVL)                                         
      GO TO 6                                                           
   7  FTC1=0.5                                                          
      FTC2=0.                                                           
    6 WRITE(6,114) WA,TA,NFQ,WBEGIN,WEND                                
C                                                                       
C    COMPUTE RANDOM PHASE ANGLES                                        
C                                                                       
      DO 31 I=1,NFQ                                                     
      IY=MOD(IX*106+1283,6075)                                                  
C*CDC*IY=IX*16777219                                                    
      IF (IY.GE.0.) GO TO 32                                            
      IY=IY+2147483647+1                                                
C*CDC*IY=IY+140737488355327+1                                           
   32 YFL=(1001*IY)/6075                                              
      YFL=YFL/1000                                                 
C*CDC*YFL=YFL*.71054273576010E-14                                       
      PA(I)=6.2832*YFL                                      
   31 IX=IY                                                       
C                                                                       
C    ACCELERATION COMPUTATIONS                                          
C                                                                       
 8603 NACCG=DUR/DELT+1.000001                                           
      IF(NCYCLE.LE.ICYCLE) GO TO 9801                                   
      WRITE(6,9008) ICYCLE,TQ(1)                                        
      WRITE (6,9567)                                                    
9801  DO 1114 KK=1,NACCG                                                
 1114 ACCG(KK)=0.                                                       
      KCHEK=1000                                                        
      DO 12   LM=1,NFQ                                                  
      IF (GWG(LM).LT.0.0) WRITE (6,3000) GWG(LM),LM                     
      GWG(LM)=ABS(GWG(LM))                                              
      AA=SQRT(GWG(LM))                                                  
      ALFA=FQ(LM)*DELT                                                  
      SINA=SIN(ALFA)                                                    
      COSA=COS(ALFA)                                                    
      SN=SIN(PA(LM))                                                    
      CN=COS(PA(LM))                                                    
      SNA=SINA*CN+COSA*SN                                               
      CNA=COSA*CN-SINA*SN                                               
      ACCG(2)=AA*SNA+ACCG(2)                                            
      DO 12   KK=3,NACCG                                                
      IF (KK.GE.KCHEK) GO TO 5012                                       
      SNO=SNA                                                           
      SNA=SNA*COSA+CNA*SINA                                             
      CNA=CNA*COSA-SNO*SINA                                             
      GO TO 12                                                          
 5012 KCHEK=KCHEK+1000                                                  
      SNA=SIN(PA(LM)+(KK-1)*ALFA)                                       
      CNA=COS(PA(LM)+(KK-1)*ALFA)                                       
   12 ACCG(KK)=AA*SNA+ACCG(KK)                                          
C                                                                       
      GO TO (3003,3003,3004,3007),ICASE                                 
C                                                                       
C    TRAPEZOIDAL INTENSITY ENVELOPE                                     
 3003 IF(ICASE.LE.1) GO TO 18                                           
      TX=TRISE                                                          
      GO TO 19                                                          
   18 TX=2.                                                             
C                                                                       
C    DEFINE MAXIMUM HEIGHTS IN TERMS OF SLOPES                          
C                                                                       
   19 DO 16 KK=2,NACCG                                                  
      TI=(KK-1)*DELT                                                    
      IF (TI.GT.TX) GO TO 15                                            
      FT=FTC1*TI                                                        
      GO TO 16                                                          
   15 IF(ICASE.LE.1) GO TO 28                                           
      IF((TI-TX-TLVL).GT.0.) GO TO 29                                   
   28 FT=1.                                                             
      GO TO 16                                                          
   29 FT=1.+(TI-TX-TLVL)*FTC2                                           
C                                                                       
C    COMPUTE ACCELERATION                                               
C                                                                       
   16 ACCG(KK)=ACCG(KK)*FT                                              
      GO TO 3011                                                        
C                                                                       
C    EXPONENTIAL INTENSITY ENVELOPE                                     
 3004 DO 3006 KK=2,NACCG                                                
      TI=(KK-1)*DELT                                                    
      FT=AO*(EXP(-ALFAO*TI)-EXP(-BETAO*TI))                             
 3006 ACCG(KK)=ACCG(KK)*FT                                              
      GO TO 3011                                                        
C                                                                       
C    COMPOUND INTENSITY ENVELOPE                                        
 3007 DO 3010 KK= 2,NACCG                                               
      TI=(KK-1)*DELT                                                    
      IF(TI.GE.TRISE) GO TO 3008                                        
      FT=(TI/TRISE)**IPOW                                               
      GO TO 3010                                                        
 3008 IF ((TI-TLVL-TRISE).LT.0.) GO TO 3009                             
      FT=EXP(-ALFAO*(TI-TLVL))                                          
      GO TO 3010                                                        
 3009 FT=1.0                                                            
 3010 ACCG(KK)=ACCG(KK)*FT                                              
 3011 CONTINUE                                                          
C                                                                       
C    COMPUTE MAX GROUND ACCELERATION BEFORE BASELINE CORRECTION         
C                                                                       
   20 AMAXIM=0.                                                         
      DO 5000 I=1,NACCG                                                 
      IF(ABS(ACCG(I)).LT.ABS(AMAXIM)) GO TO 5000                        
      AMAXIM=ACCG(I)                                                    
      TMAXIM=(I-1)*DELT                                                 
 5000 CONTINUE                                                          
      IF (NCYCLE.GT.ICYCLE) GO TO 8504                                  
      WRITE(6,5200) AMAXIM,TMAXIM                                       
 8504 T1=-DELT*0.5                                                      
C                                                                       
C    JUSTIFY ACCG TO ZERO FINAL VELOCITY                                
C                                                                       
      BETA1=0.                                                          
      BETA2=0.                                                          
      BETA3=0.                                                          
      VEL=0.                                                            
      DO 4300 IZ=1,NACCG                                                
      VEL=VEL+ACCG(IZ)*DELT                                             
      T1=T1+DELT                                                        
      BETA1=BETA1+VEL*T1                                                
      BETA2=BETA2+VEL*T1*T1                                             
 4300 BETA3=BETA3+VEL*T1*T1*T1                                          
      BETA1=BETA1*DELT/(T1*T1*T1)                                       
      BETA2=BETA2*DELT/(T1*T1*T1*T1)                                    
      BETA3=BETA3*DELT/(T1*T1*T1*T1*T1)                                 
      C1=300.*BETA1-900.*BETA2+630.*BETA3                               
      C2=(-1800.*BETA1+5760.*BETA2-4200.*BETA3)/T1                      
      C3=(1890.*BETA1-6300.*BETA2+4725.*BETA3)/(T1*T1)                  
      DO 4310 IZ=1,NACCG                                                
      TI=(IZ-1)*DELT                                                    
 4310 ACCG(IZ)=ACCG(IZ)-C1-C2*TI-C3*TI*TI                               
C                                                                       
C    GET MAXIMUM GROUND ACCELERATION                                    
C                                                                       
      GAMX=ACCG(1)                                                      
      VEL=0.                                                            
      VAMX=0.                                                           
      DISP=0.                                                           
      DMAX=0.                                                           
      LL1=0                                                             
      GAMX=ABS(GAMX)                                                    
      DO 59 LL=2,NACCG                                                  
      GAMY=ABS(ACCG(LL))                                                
      VEL=VEL+ACCG(LL)*DELT                                             
      DISP=DISP+VEL*DELT                                                
      DAMY=ABS(DISP)                                                    
      VAMY=ABS(VEL)                                                     
      IF (DAMY.LE.DMAX) GO TO 52                                        
   53 DMAX=DAMY                                                         
   52 IF (VAMY.LE.VAMX) GO TO 56                                        
      VAMX=VAMY                                                         
   56 IF(GAMY.LE.GAMX) GO TO 59                                         
   58 GAMX=GAMY                                                         
      LL1=LL                                                            
   59 CONTINUE                                                          
C                                                                       
C    NO SCALING OF THE ENTIRE TIME HISTORY IS DONE BUT PEAKS ARE        
C    ADJUSTED IN ORDER TO HAVE ONLY ONE PEAK EQUAL TO THE SPECIFIED     
C    MAXIMUM GROUND ACCELERATION.                                       
      TTT=ABS(GAMX/AGMX)                                                
      IF(TTT.LE.1.) GO TO 1112                                          
      DO 111 K1=1,NACCG                                                 
      DAR=ABS(ACCG(K1))-AGMX                                            
      IF(DAR.LE.0.) GO TO 111                                           
      ACCG(K1)=ACCG(K1)/TTT                                             
  111 CONTINUE                                                          
      GO TO 1113                                                        
 1112 ACCG(LL1)=ACCG(LL1)/TTT                                           
 1113 GAMX=AGMX/386.4                                                   
      LIM=NDAMP                                                         
      IF (ICYCLE.LT.NCYCLE) LIM=1                                       
C                                                                       
C    CHECK ACCG DIMENSIONS                                              
C                                                                       
      ICK=NACCG+2.*TQ(NKK)/DELT                                         
      IF (ICK.GE.8000) WRITE (6,34) ICK                                 
      IF (ICK.GE.8000) GO TO 9003                                       
C                                                                       
C    RESPONSE CALCULATION AND PLOTTING                                  
C                                                                       
      CALL SPECT(PLTVMX,TMD,ACCG,NACCG,DELT,TQ,NKK,AMOR,LIM)            
      IF (IPCH.EQ.0) GO TO 35                                           
      WRITE (6,27) ICYCLE                                                  
      WRITE (6,888) (GWK(NKK-I+1),I=1,NKK)                                 
   35 CONTINUE                                                          
      IF(NCYCLE.LE.ICYCLE) GO TO 44                                     
C                                                                       
C    CYCLING PROCEDURE WHICH MODIFIES G(W) TO SMOOTHEN THE CALCULATED   
C    RESPONSE SPECTRUM                                                  
C                                                                       
      SUMPOS = 0.                                                       
      SUMNEG = 0.                                                       
      DO 43 I=1,NKK                                                     
      AMULT=SV(I)/PLTVMX(1,I)                                           
      RATIOS = ABS (1./AMULT)*100.                                      
      PERCEN(I) =      RATIOS - 100.                                    
      WRITE(6,8901) TQ(I),FRQ(I),GWK(NKK-I+1),SV(I),PLTVMX(1,I),        
     * PERCEN(I),TMD(1,I),I                                             
      J=NKK-I+1                                                         
10002 ANEWGK(J) = GWK(J)*AMULT*AMULT                                    
      AINCRM = ANEWGK(J)-GWK(J)                                         
      IF (AINCRM.GE.0.) SUMPOS = SUMPOS+AINCRM                          
      IF (AINCRM.LT.0.) SUMNEG = SUMNEG-AINCRM                          
   43 CONTINUE                                                          
      IF (SUMNEG.LE.1.E-8) GO TO 213                                    
      FACTOR = SUMPOS/SUMNEG                                            
      WRITE (6,10000) SUMPOS,SUMNEG,FACTOR                              
      DO 211  I=1,NKK                                                   
  211 GWK(I) = ANEWGK(I)                                                
      GO TO 100                                                         
C                                                                       
C    OPTION THAT MAKES NO CHANGES IN POSITIVE INCREMENTS WHEN SUMNEG    
C    IS LESS THAN 1.0E-8                                                
C                                                                       
  213 DO 214  I=1,NKK                                                   
  214 GWK(I) = ANEWGK(I)                                                
      GO TO 100                                                         
C                                                                       
C    WRITE MAXIMUM RESPONSE VALUE                                       
C                                                                       
   44 WRITE(6,120)GAMX,VAMX,DMAX                                        
      DO 17 I=1,NACCG                                                   
   17 ACCG(I)=ACCG(I)/386.4                
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!                             
c      WRITE(6,5203) (ACCG(I),I=1,NACCG)                                 
	do KKKK=1, NACCG
		write(6,*) DELT*(KKKK-1), ACCG(KKKK)
	end do
	open (55,file="EQ01.txt")
		do KKKK=1, NACCG
			write(55,*) DELT*(KKKK-1), ACCG(KKKK)
		end do
		if(NACCG<4001) then
			do KKKK=NACCG+1, 4001
				write(55,*) DELT*(KKKK-1), 0.	
			end do
		end if
	close(55)
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      IF (IPCH.EQ.0) GO TO 36                                           
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
c      WRITE (6,4111) (ACCG(I),I=1,NACCG)                                   
	do KKKK=1, NACCG
		write(6,*) DELT*(KKKK-1), ACCG(KKKK)
	end do
	open (55,file="EQ01.txt")
		do KKKK=1, NACCG
			write(55,*) DELT*(KKKK-1), ACCG(KKKK)
		end do
		if(NACCG<4001) then
			do KKKK=NACCG+1, 4001
				write(55,*) DELT*(KKKK-1), 0.	
			end do
		end if
	close(55)
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      KOUNT=0                                                           
      JK=0                                                              
      WRITE (6,1) TITLE                                                    
      WRITE (6,22) NDAMP,NKK                                               
      WRITE (6,13) (TQ(I),I=1,NKK)                                         
 9101 KOUNT=KOUNT+1                                                     
      J=JK+1                                                            
      JK=JK+8                                                           
      IF (JK.LT.NACCG) GO TO 9103                                       
      JJ=NACCG+1                                                        
      DO 9104 K=JJ,JK                                                   
 9104 ACCG(K)=0.0                                                       
 9103 WRITE (6,301) (ACCG(I),I=J,JK),KOUNT                                 
      IF (JK.LT.NACCG) GO TO 9101                                       
      TEMP=99.                                                          
      KOUNT=KOUNT+1                                                     
      WRITE (6,9102) TEMP,KOUNT                                            
   36 CONTINUE                                                          
      DO 9012 LL=1,NDAMP                                                
      WRITE(6,4535) AMOR(LL)                                            
      IF (IPCH.EQ.0) GO TO 37                                           
      CAM=AMOR(LL) * 100.                                               
      WRITE (6,9016) CAM                                                   
      WRITE (6,888) (PLTVMX(LL,N),N=1,NKK)                                 
   37 CONTINUE                                                          
 9012 WRITE (6,4340)(TQ(KK),FRQ(KK),PLTVMX(LL,KK),TMD(LL,KK),KK,      
     $    KK=1,NKK)                                                     
      IF (NRES.EQ.0) GO TO 100                                          
      WRITE(6,9567)                                                     
      DO 23 I=1,NKK                                                     
      AMULT=SV(I)/PLTVMX(1,I)                                           
      RATIOS = ABS (1./AMULT)*100.                                      
      PERCEN(I) =      RATIOS - 100.                                    
   23 WRITE(6,8901) TQ(I),FRQ(I),GWK(NKK-I+1),SV(I),PLTVMX(1,I),        
     * PERCEN(I),TMD(1,I),I                                             
      DO 21 II=1,NDAMP                                                  
      DO 21 JJ=1,NKK                                                    
   21 PLTVMX(II,JJ)=ABS(PLTVMX(II,JJ))                                  
      NFC=2                                                             
      DO 1000 II=1,NDAMP                                                
      DO 1001 J=1,NKK                                                   
 1001 SI(J)=PLTVMX(II,J)                                                
      XAMOR=AMOR(II)                                                    
      CALL DIB2 (NFC,4,1,0,NKK,TS,TL,YMIN,YMAX,1.,1.,0,0,0,0,-2,-2,     
     $TQ,SI,SV,TIX,TITX,TITY,36,36,36,0,0.,XAMOR)                       
 1000 CONTINUE                                                          
  100 CONTINUE                                                          
  585 CONTINUE                                                          
      IF(NKK.GT.0)GOTO1100                                              
 1100 CALL PLTND(KIKI)                                                  
C     CALL EXIT                                                         
      return                                                              
C                                                                       
    1 FORMAT (20A4)                                                     
    2 FORMAT(1H ,//,2X,20A4)                                            
   13 FORMAT (10F8.4)                                                   
   22 FORMAT (2I10)                                                     
   27 FORMAT (1X,14HGWK FOR CYCLE ,I2)                                  
   30 FORMAT (//,7X,17HTIME INCREMENT = ,F5.3)                          
   34 FORMAT (2X,55HACCG ARRAY NOT ENOUGH FOR NACCG+2*(LARGEST PERIOD)/D
     *T =,I5)                                                           
   60 FORMAT (//,10X,34HA NEW PHASE ANGLE SET WITH SEED = ,I10)         
  106 FORMAT (7X,30HEXPECTED MAXIMUM GROUND ACC = ,F7.2,14H IN./SEC./SEC
     *. )                                                               
  107 FORMAT (7X,7HTRISE =,F7.2,2X,8HTLEVEL =,F7.2,2X,10HDURATION =,F7.2
     *)                                                                 
  114 FORMAT (//,10X,29HCENTRAL CIRCULAR FREQUENCY = ,F10.4,13H RADIANS/
     *SEC.,//,10X,17HCENTRAL PERIOD = ,F8.4,8H SECONDS,//,10X,25HNUMBER 
     *OF PHASE ANGLES = ,I5,//,10X,29HLOWEST FREQUENCY IN MOTION = ,F10.
     *5,13H RADIANS/SEC.,//,10X,30HHIGHEST FREQUENCY IN MOTION = ,F10.5,
     *13H RADIANS/SEC.)                                                 
  120 FORMAT (//,10X,30HMAXIMUM GROUND ACCELERATION = ,F6.3,5H G*S ,//, 
     * 10X,26HMAXIMUM GROUND VELOCITY = ,F6.3,9H IN./SEC.,//,           
     * 10X,30HMAXIMUM GROUND DISPLACEMENT = ,F6.3,4H IN. ,//,           
     * 20X,29HSIMULATED GROUND ACCELERATION,//)                         
  129 FORMAT (2F10.4,I10,8I5)                                           
  134 FORMAT(7X,15HSTATIONARY CASE)                                     
  135 FORMAT(7X,59HNON-STATIONARY IN INTENSITY BUT STATIONARY IN FREQ SP
     /ECTRUM)                                                           
  301 FORMAT (8F9.5,I8)                                                 
  888 FORMAT (6F13.3)                                                   
 3000 FORMAT (1X,20HGWG NEGATIVE. EQUALS ,E10.3,2X,10HFOR LM OF ,I5)    
 3020 FORMAT (I5,6F10.4,I5)                                             
 4111 FORMAT(8F9.5)                                                     
 4262 FORMAT (2F10.4)                                                   
 4340 FORMAT (1X,4F14.4,I10)                                            
 4535 FORMAT (1H ,1X,10HDAMPING = ,F6.3,///,9X,6HPERIOD,6X,9HFREQUENCY, 
     * 7X,8HRESPONSE,6X,4HTIME,//)                                      
 5200 FORMAT (1H ,//,10X,29HMAX. ACCEL. BEFORE CORRECTION,F12.5,//      
     *       10X,7HAT TIME,F12.5,//)                                    
 5203 FORMAT (5H     ,15F8.4)                                           
 6008 FORMAT (/,11X,31HSTANDARD DEVIATION OF PROCESS = ,F7.4,4H G*S)    
 6016 FORMAT (//,11X,23HORIGINAL POWER SPECTRUM,//,11X,6HPERIOD,8X,     
     *  9HFREQUENCY,7X,8HSPECTRUM,12X,1HR,//)                           
 7020 FORMAT(8G10.0)                                                    
 8901 FORMAT (5(4X,F12.3),4X,F12.1,4H PCT,2X,F12.3,I10)                 
 8902 FORMAT (//,10X,24H CENTRAL FREQUENCY WC = ,F10.3,//,10X,26H DISPER
     *SION PARAMETER Q = ,F10.3,/)                                      
 9008 FORMAT (1H ,30X,12HCYCLE NUMBER ,I2,20X,25HLOWEST MODIFIED PERIOD
     * = ,F10.4,2X,7HSECONDS,//)                                        
 9015 FORMAT(10F8.4)                                                    
 9016 FORMAT (1X,7HDAMPING,2X,F4.1,7HPERCENT)                           
 9102 FORMAT (F9.6,63X,I8)                                              
 9567 FORMAT (///,9X,6HPERIOD,8X,9HFREQUENCY,4X,13HPOW.SPEC.DEN.,5X,    
     * 12HDES.RESPONSE,4X,12HCAL.RESPONSE,7X,10HDIFFERENCE,9X,4HTIME,//)
10000 FORMAT (//,10X,8HSUMPOS =,F12.3,10X,8HSUMNEG =,F12.3,10X,8HFACTOR 
     *=,F12.3)                                                          
C                                                                       
      END                                                               
      SUBROUTINE PLTX2(XMIN,XMAX,X,NPOINT)                              
      DIMENSION X(1)                                                    
      POINT=NPOINT-1                                                    
      SPACE=ALOG10(XMAX/XMIN)/POINT                                     
      X(1)=XMIN                                                         
      DO 1 I=2,NPOINT                                                   
      AI=I-1                                                            
      EXPO=SPACE*AI                                                     
    1 X(I)=XMIN*10.**EXPO                                               
      X(NPOINT)=XMAX                                                    
      RETURN                                                            
       END                                                              
      SUBROUTINE POLATE (N,M,XIN,YIN,XOUT,YOUT)                         
      DIMENSION XIN(1),YIN(1),XOUT(1),YOUT(1)                           
      J=1                                                               
      IF (XIN(1)-XOUT(1)) 2,2,100                                       
    2 IF (XIN(N)-XOUT(M)) 100,3,3                                       
    3 DO 30 I=1,M                                                       
    6 IF (XOUT(I)-XIN(J)) 5,40,4                                        
    4 J=J+1                                                             
      GO TO 6                                                           
    5 J=J-1                                                             
      YTEST=(ALOG(YIN(J+1))-ALOG(YIN(J)))*(ALOG(XOUT(I))-ALOG(XIN(J)))/ 
     1 (ALOG(XIN(J+1))-ALOG(XIN(J)))+ALOG(YIN(J))                       
      YOUT(I)=EXP(YTEST)                                                
      GO TO 30                                                          
   40 YOUT(I)=YIN(J)                                                    
   30 CONTINUE                                                          
      RETURN                                                            
  100 WRITE (6,20)                                                      
   20 FORMAT (1H ,1X, 53HPROGRAM STOP. FUNCTION UNDEFINED IN DESIRED INT
     1ERVAL )                                                           
C     CALL EXIT                                                         
      return                                                              
      END                                                               
      SUBROUTINE SVGW(NKK,W,GW,SV,S,B,WC,Q,P,XLAM0,WCP,QP,RR)           
      DIMENSION GW(1),W(1),SV(1),RR(1)                                  
      PI=3.14159                                                        
      PI2=6.2831852                                                     
      GSUM=0.                                                           
      DO 1000 I=1,NKK                                                   
      NW=NKK-I+1                                                        
      POW=2.*B*W(I)*S                                                   
      IF(POW.GT.50.0) GO TO 610                                         
      TRANS=1.-EXP(-POW)                                                
      GO TO 611                                                         
  610 TRANS=1.                                                          
  611 BS=B/TRANS                                                        
      WCYS=W(I)                                                         
      QYS=SQRT(4.0*BS/PI)                                               
      XSP=-WCYS*S/(PI2*ALOG(P))                                         
      RSTAR=SQRT(2.*ALOG(2.*XSP))                                       
      ET=-RSTAR*QYS*SQRT(PI/2)                                          
      ARG=2.*XSP*(1.-EXP(ET))                                           
      RSP=SQRT(2.*ALOG(ARG))                                            
      RR(I)=RSP                                                         
      GW(I)=(4.*BS/(W(I)*PI))*((SV(NW)*W(I)/RSP)**2-GSUM)               
      IF(GW(I).LE.0.01)GW(I)=0.01                                       
      IF(I.GT.1)GO TO 140                                               
      GSUM=0.5*W(1)*GW(1)                                               
      GO TO 1000                                                        
 140  GSUM=GSUM+GW(I)*(W(I)-W(I-1))                                     
 1000 CONTINUE                                                          
      WCP=0.0                                                           
      QP=0.0                                                            
      XLAM0=0.                                                          
      XLAM1=0.                                                          
      XLAM2=0.                                                          
      DO 5 I=2,NKK                                                      
      DUMX=(GW(I)+GW(I-1))/2.                                           
      DUMY=W(I)-W(I-1)                                                  
      IF(GW(I)-GW(I-1)) 10,15,15                                        
   10 A=GW(I)                                                           
      B=GW(I-1)                                                         
      WBAR=DUMY*(2.*B+A)/(3.*(A+B))                                     
      WSTAR=W(I)-WBAR                                                   
      GO TO 16                                                          
   15 A=GW(I-1)                                                         
      B=GW(I)                                                           
      WBAR=DUMY*(2.*B+A)/(3.*(A+B))                                     
      WSTAR=W(I-1)+WBAR                                                 
   16 AREA=DUMX*DUMY                                                    
      XLAM0=XLAM0+AREA                                                  
      XLAM1=XLAM1+WSTAR*AREA                                            
    5 XLAM2=XLAM2+(WSTAR**2)*AREA                                       
      WCP=SQRT(XLAM2/XLAM0)                                             
      RATIO=(XLAM1**2)/(XLAM0*XLAM2)                                    
      QP=SQRT(1.-RATIO)                                                 
      RETURN                                                            
      END                                                               
      SUBROUTINE GWPLOT(NKK,TS,TL,GMIN,GMAX,TQ,GW,TITX,TITLO,YTITL)     
      DIMENSION TQ(1),GW(1),TITX(1),TITLO(1),YTITL(1)                   
      IF (GMAX.LE.70.0) GO TO 3                                         
      IF (GMAX.LE.200.0) GO TO 2                                        
      DY=20.0                                                           
      GO TO 4                                                           
    2 DY=10.0                                                           
      GO TO 4                                                           
    3 DY=2.0                                                            
    4 CONTINUE                                                          
C     ESTABLISH SEMILOG COORDINATES                                     
      CALL SMXYV(1,0)                                                   
C     ESTABLISH MARGINS                                                 
      CALL SETMIV(150,100,150,150)                                      
C     ESTABLISH GRID                                                    
      CALL GRID1V(1,TS,TL,GMIN,GMAX,1.0,DY,0,5,0,5,-2,-2)               
C     WRITE Y AXIS LABEL                                                
      CALL RITE2V(125,250,1000,90,2,28,1,YTITL,NLAST)                   
C     WRITE X AXIS LABEL                                                
      CALL RITE2V(300,125,1000,0,2,36,1,TITX,NLAST)                     
C     WRITE TITLE                                                       
      CALL RITE2V(250,925,1000,0,2,28,1,TITLO,NLAST)                    
C     JOIN POINTS WITH STRAIGHT LINES                                   
      NKKM1=NKK-1                                                       
      DO 1 I=1,NKKM1                                                    
      X1=TQ(I)                                                          
      X2=TQ(I+1)                                                        
      II=NKK+1-I                                                        
      Y1=GW(II)                                                         
      Y2=GW(NKK-I)                                                      
C     IX1=NXV(X1)                                                       
C     IY1=NYV(Y1)                                                       
C     IX2=NXV(X2)                                                       
C     IY2=NYV(Y2)                                                       
C     CALL LINEV(IX1,IY1,IX2,IY2)                                       
  1   CONTINUE                                                          
      RETURN                                                            
      END                                                               
      SUBROUTINE DUMMY(W,FOUT,NKK,WB,GWK,MM)                            
      DIMENSION WB(1),GWK(1)                                            
      JAY=MM                                                            
    1 IF(W-WB(JAY)) 5,4,2                                               
    2 JAY=JAY+1                                                         
      IF (JAY.LE.NKK) GO TO 1                                           
      FOUT=GWK(NKK)                                                     
      GO TO 6                                                           
    4 FOUT=GWK(JAY)                                                     
      MM=JAY                                                            
      GO TO 6                                                           
    5 MM=JAY-1                                                          
      IF (MM.LE.0) GO TO 4                                              
      SLOPE=(GWK(JAY)-GWK(JAY-1))/(WB(JAY)-WB(JAY-1))                   
      FOUT=GWK(JAY-1)+SLOPE*(W-WB(JAY-1))                               
    6 CONTINUE                                                          
      RETURN                                                            
      END                                                               
      SUBROUTINE DUMMX(W,FOUT,NKK,WB,GWK,MM)                            
      DIMENSION WB(1),GWK(1)                                            
      JAY=MM                                                            
    1 IF(W-WB(JAY)) 5,4,2                                               
    2 JAY=JAY+1                                                         
      IF (JAY.LE.NKK) GO TO 1                                           
      FOUT=GWK(NKK)                                                     
      GO TO 6                                                           
    4 FOUT=GWK(JAY)                                                     
      MM=JAY                                                            
      GO TO 6                                                           
    5 MM=JAY-1                                                          
      IF (MM.LE.0) GO TO 4                                              
      X=(WB(JAY)+WB(JAY-1))/2.                                          
      IF(W-X) 7,7,8                                                     
    7 FOUT=GWK(JAY-1)                                                   
      GO TO 6                                                           
    8 FOUT=GWK(JAY)                                                     
    6 CONTINUE                                                          
      RETURN                                                            
      END                                                               
      SUBROUTINE SPECT (VMAX,TA,GA,N,DEL,PD,IP,DMP,ID)                  
C                                                                       
C  SUBROUTINE FOR COMPUTATION OF SPECTRA FROM EARTHQUAKE RECORD         
C  DIGITIZED AT EQUAL TIME INTERVALS                                    
C                                                                       
      DIMENSION VMAX(10,300),TA(10,300),GA(6001),PD(300),DMP(10),       
     1          A(2,2),B(2,2),TY(3),X(3),G(2)                           
      DO 6 J=1,ID                                                       
      D=DMP(J)                                                          
      DO 6  K=1,IP                                                      
      P=PD(K)                                                           
      IF (P.LT.0.001) P=0.001                                           
      W=6.2831854/P                                                     
C                                                                       
C  CHOICE OF INTERVAL OF INTEGRATION                                    
C                                                                       
      DELP=P/10.                                                        
      L=DEL/DELP+1.-1.E-5                                               
      DELT=DEL/L                                                        
C                                                                       
C  COMPUTATION OF MATRICES A AND B                                      
C                                                                       
      CALL PCN04(D,W,DELT,A,B)                                          
C                                                                       
C  INITIATION                                                           
C                                                                       
      X(1)=0.                                                           
      X(2)=0.                                                           
      DMAX=0.                                                           
      I=1                                                               
      DW=2.*W*D                                                         
      W2=W**2                                                           
      IA=2.*P/DELT+1.E-05                                               
C                                                                       
C  COMPUTATION OF RESPONSE                                              
C                                                                       
      L1=0                                                              
    1 SL=(GA(I+1)-GA(I))/ L                                             
      DO 5 M=1,L                                                        
      G(1)= GA(I)+SL*(M-1)                                              
      G(2)= GA(I)+SL*M                                                  
      TY(1)=A(1,1)*X(1)+A(1,2)*X(2)-B(1,1)*G(1)-B(1,2)*G(2)             
      TY(2)=A(2,1)*X(1)+A(2,2)*X(2)-B(2,1)*G(1)-B(2,2)*G(2)             
      L1=L1+1                                                           
      TIME=(L1-1)*DELT                                                  
C                                                                       
C  MONITORING THE MAX. VALUES                                           
C                                                                       
      IF (ABS(TY(1)).LE.ABS(DMAX)) GO TO 2                              
      DMAX=TY(1)                                                        
      TD=TIME                                                           
    2 X(1)=TY(1)                                                        
    5 X(2)=TY(2)                                                        
C                                                                       
C  TEST FOR END OF INTEGRATION                                          
C                                                                       
      I=I+1                                                             
      IF (I.EQ.N) GO TO 7                                               
      GO TO 8                                                           
    7 VEND=X(2)                                                         
    8 IF (I.EQ.(N+IA)) GO TO 10                                         
      IF (I.GE.N) GO TO 9                                               
      GO TO 1                                                           
    9 GA(I+1)=0.                                                        
      GO TO 1                                                           
   10 CONTINUE                                                          
      VMAX(J,K)=W*DMAX                                                  
      TA(J,K)=TD                                                        
    6 CONTINUE                                                          
      RETURN                                                            
      END                                                               
      SUBROUTINE PCN04(D,W,DELT,A,B)                                    
C                                                                       
C  SUBROUTINE FOR COMPUTATION OF MATRICES A AND B                       
C                                                                       
      DIMENSION A(2,2),B(2,2)                                           
      DW=D*W                                                            
      D2=D**2                                                           
      A0=EXP(-DW*DELT)                                                  
      A1=W*SQRT(1.-D2)                                                  
      AD1=A1*DELT                                                       
      A2=SIN(AD1)                                                       
      A3=COS(AD1)                                                       
      W2=W**2                                                           
      A4=(2.*D2-1.)/W2                                                  
      A5=D/W                                                            
      A6=2.*A5/W2                                                       
      A7=1./W2                                                          
      A8=(A1*A3-DW*A2)*A0                                               
      A9=-(A1*A2+DW*A3)*A0                                              
      A10=A8/A1                                                         
      A11=A0/A1                                                         
      A12=A11*A2                                                        
      A13=A0*A3                                                         
      A14=A10*A4                                                        
      A15=A12*A4                                                        
      A16=A6*A13                                                        
      A17=A9*A6                                                         
      A(1,1)=A0*(DW*A2/A1+A3)                                           
      A(1,2)=A12                                                        
      A(2,1)=A10*DW+A9                                                  
      A(2,2)=A10                                                        
      B(1,1)=(-A15-A16+A6)/DELT-A12*A5-A7*A13                           
      B(1,2)=(A15+A16-A6)/DELT+A7                                       
      B(2,1)=(-A14-A17-A7)/DELT-A10*A5-A9*A7                            
      B(2,2)=(A14+A17+A7)/DELT                                          
      RETURN                                                            
      END                                                               
      SUBROUTINE DIB2 (NFC,IND,NGRAPH,NGD,NPOINT,XL,XR,YB,YT,DX,DY,     
     $N,M,I,J,NX,NY,X,Y,Z,TIT,TITX,TITY,NT,NTX,NTY,NPT,PTMRK,XAMOR)     
      DIMENSION X(1),Y(1),Z(1),TIT(1),TITX(1),TITY(1),PTMRK(1)          
      INDA=0                                                            
      GO TO (1,2,3,4),IND                                               
    1 CALL SMXYV(0,0)                                                   
      GO TO 5                                                           
    2 CALL SMXYV(0,1)                                                   
      GO TO 5                                                           
    3 CALL SMXYV(1,0)                                                   
      GO TO 5                                                           
    4 CALL SMXYV(1,1)                                                   
    5 CONTINUE                                                          
      CALL SETMIV(150,100,150,150)                                      
      IF(NFC-1) 11,10,20                                                
   10 NFA=2                                                             
      GO TO 30                                                          
   20 NFA=4                                                             
   30 CALL GRID1V(NFA,XL,XR,YB,YT,DX,DY,N,M,I,J,NX,NY)                  
      CALL RITE2V(125,250,1000,90,2,NTY,1,TITY,NLAST)                   
      CALL RITE2V(300,125,1000,0,2,NTX,1,TITX,NLAST)                    
      CALL RITE2V(250,925,1000,0,2,NT,1,TIT,NLAST)                      
      CALL LABLV (XAMOR,750,880,6,1,1)                                  
   11 CALL INCRV(8,4)                                                   
      NAU=NGRAPH+NGD                                                    
      IF(NAU) 401,401,400                                               
  400 DO 7 II=1,NAU                                                     
      NAUX=NPOINT-1                                                     
      DO 8 K=1,NAUX                                                     
      IAUX=(II-1)*NPOINT+K                                              
      X1=X(K)                                                           
      Z1=Z(K)                                                           
      X2=X(K+1)                                                         
      Z2=Z(K+1)                                                         
      Y1=Y(IAUX)                                                        
      Y2=Y(IAUX+1)                                                      
      IF(Y1-YT) 100,100,101                                             
  100 IF(Y2-YT) 110,110,103                                             
  103 X2=(X2-X1)*(YT-Y1)/(Y2-Y1)+X1                                     
      Y2=YT                                                             
      GO TO 110                                                         
  101 IF(Y2-YT) 104,104,105                                             
  104 X1=(X2-X1)*(YT-Y1)/(Y2-Y1)+X1                                     
      Y1=YT                                                             
      GO TO 110                                                         
  105 INDA=1                                                            
  110 CONTINUE                                                          
      IF(Y1-YB) 200,201,201                                             
  200 IF(Y2-YB) 205,203,203                                             
  205 INDA=1                                                            
      GO TO 210                                                         
  203 X1=(X2-X1)*(YB-Y1)/(Y2-Y1)+X1                                     
      Y1=YB                                                             
      GO TO 210                                                         
  201 IF(Y2-YB) 204,210,210                                             
  204 X2=(X2-X1)*(YB-Y1)/(Y2-Y1)+X1                                     
      Y2=YB                                                             
  210 CONTINUE                                                          
      IF(INDA) 303,303,302                                              
  303 IF(II-NGRAPH) 300,300,301                                         
  300 CALL LINEV(NX,NY,NX,NY)                                           
      CALL LINEV(NX,NY,NX,NY)                                           
      CALL DOTLNV(NX,NY,NX,NY)                                          
      GO TO 302                                                         
  301 CALL DOTLNV(NX,NY,NX,NY)                                          
      CALL DOTLNV(NX,NY,NX,NY)                                          
  302 INDA=0                                                            
    8 CONTINUE                                                          
    7 CONTINUE                                                          
  401 IF(NPT) 402,402,403                                               
  403 LL=NPOINT*NPT                                                     
      DO 500 I=1,NPOINT                                                 
      CALL APLOTV(LL,X(I),Y(I),0,NPOINT,NPT,PTMRK,IERR)                 
  500 CALL APLOTV(LL,X(I),Y(I),0,NPOINT,NPT,PTMRK,IERR)                 
  402 RETURN                                                            
      END                                                               
      SUBROUTINE PLTND (KIKI)                                           
C    DUMMY PLOT SUBROUTINE                                              
      RETURN                                                            
      END                                                               
      SUBROUTINE SMXYV (I,J)                                            
C    DUMMY PLOT SUBROUTINE                                              
      RETURN                                                            
      END                                                               
      SUBROUTINE SETMIV (J,K,L,M)                                       
C    DUMMY PLOT SUBROUTINE                                              
      RETURN                                                            
      END                                                               
      SUBROUTINE GRID1V (NFA,XL,XR,YB,YT,DX,DY,N,M,I,J,NX,NY)           
C    DUMMY PLOT SUBROUTINE                                              
      RETURN                                                            
      END                                                               
      SUBROUTINE RITE2V (II,JJ,KK,I,J,K,IJ,IK,IL)                       
C    DUMMY PLOT SUBROUTINE                                              
      RETURN                                                            
      END                                                               
      SUBROUTINE LABLV (XAMOR,X,Y,Z,I,J)                              
C    DUMMY PLOT SUBROUTINE                                              
      RETURN                                                            
      END                                                               
      SUBROUTINE INCRV (I,J)                                            
C    DUMMY PLOT SUBROUTINE                                              
      RETURN                                                            
      END                                                               
      SUBROUTINE LINEV (N1,N2,N3,N4)                                    
C    DUMMY PLOT SUBROUTINE                                              
      RETURN                                                            
      END                                                               
      SUBROUTINE DOTLNV (N1,N2,N3,N4)                                   
C    DUMMY PLOT SUBROUTINE                                              
      RETURN                                                            
      END                                                               
      SUBROUTINE APLOTV (LL,X,Y,I,N,NPT,P,IERR)                         
C    DUMMY PLOT SUBROUTINE                                              
      RETURN                                                            
      END                                                               



!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 平均加速度法计算单自由度SDOF系统地震响应计算源程序
! Sourse code for seismic response of SDOF system with average acceleration method.
! 输入数组EQ(:,2)  地震加速度记录，时间，加速度
! Input Array: EQ(:,2) Earthquake acceleration record: Time, Acceleration
! 输出数组 Disp(:)
! Output Array: Disp(:)
! 程序编写：陆新征，清华大学土木工程系，luxinzheng@sina.com
! By Xinzheng LU, Civil Engineering of Tsinghua University, luxinzheng@sina.com

	subroutine GetMAG()
	real*8 EQ(4000,2),Disp(4000),xMAG(300)
	real*8 Te, Damp

	open(55,file="EQ01.txt")
		do I=1, size(EQ,1)
			read(55,*) EQ(I,:)
		end do
	close(55)

	open(66,file='xMAG.txt')
!	write(*,'(2A10)') "  T  ", "  Sa  "
	write(*,*) "Please wait for a moment"

	do II=1, 300
		Te=real(II)*0.01d0  ! Period of system
	
		Damp=0.05d0; ! Damping ratio
		call Average_Acce(EQ,Disp,Te, Damp,size(EQ,1))
		xMaxDisp=maxval(abs(Disp))
		pi=4.d0*atan(1.d0)
		Stiff=4.d0*pi*pi/Te/Te;
		Disp0=1./Stiff ! PGA=1.0，计算静力作用的位移数值
		xMAG(II)=xMaxDisp/Disp0;
!		write(*,'(2F10.4)') Te, xMAG(II)
		write(66,*) Te, xMAG(II)

	end do

	close(55)

	return
	end subroutine

	subroutine Average_Acce(EQ,Disp,Te, Damp,N)
	implicit none
	integer :: N
	real*8 EQ(N,2)
	real*8 Disp(N)
	real*8 Te, Damp
	real*8 k, m, c, Omega,pi
	real*8 a1,a2,v1,v2,d1,d2
	real*8 dt
	integer I,J, STEP

	STEP=100 ! Number of substep for integration


	pi=4.d0*atan(1.d0)

	m=1.0;
	K=4.d0*pi*pi*m/Te/Te;

	Omega=2.d0*pi/Te
	
	c=2.d0*Damp*sqrt(k*m);

	a1=0.d0;	a2=0.d0;
	v1=0.d0;	v2=0.d0;
	d1=0.d0;	d2=0.d0;
	Disp(1)=0.d0;

	do I=2, N
		dt=(EQ(I,1)-EQ(I-1,1))/real(STEP);
		do J=1, STEP
			a1=a2; v1=v2; d1=d2;

			a2=EQ(I-1,2)+(EQ(I,2)-EQ(I-1,2))*real(J)/real(STEP)
			a2=a2+c/m*(v1+0.5d0*a1*dt)+k/m*(d1+v1*dt+0.25d0*a1*dt*dt);
			a2=-a2/(1.d0+0.5d0*c/m*dt+0.25d0*k/m*dt*dt)

			v2=v1+0.5d0*(a1+a2)*dt;
			d2=d1+v1*dt+0.25d0*(a1+a2)*dt*dt
		end do
		Disp(I)=d2
	end do

	return
	end subroutine


	subroutine BuildSpecturm(Tg)
		real*8 T(64), Sa(64), Sv(64)
		real*8 Sv_inch(64) ! 英寸单位
		do I=1,11
			T(I)=(I-1)*0.01d0;
			Sa(I)=0.45+(1.-0.45)*T(I)/0.1;
		end do
		T(12)=Tg; Sa(12)=1;
		do I=13,64
			T(I)=T(I-1)+0.05;
			if(I==64.and. T(I)<3.) T(I)=3.
			Sa(I)=(Tg/T(I))**0.9	
		end do	
		do I=1, size(T)
			Sv(I)=Sa(I)*T(I)/2./3.1415926535;
			Sv_inch(I)=Sv(I)*9.8*1000/25.4;	
		end do
		open(55,file='Site.txt')
			do I=2, size(T)
			write(55,'(2F10.5)') T(I), Sv_inch(I);
			end do
		close(55)

		open(55,file='Site_Sa.txt')
			do I=2, size(T)
			write(55,'(2F10.5)') T(I), Sa(I);
			end do
		close(55)
		return
	end subroutine
	
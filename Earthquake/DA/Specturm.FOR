!     绘制反应谱
!     By LuXZ@Tsinghua
!
	program Main

	!open(55,file="DataOut.Txt")
	!	read(55,*) xLuXZT1, xLuXZT2, xLuXZT3 , Tg
	!close(55)
	

	call GetMAG() ! 计算实际反应谱

	stop
	end program


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
	real*8 EQ(100000,2),Disp(100000),xMAG(600)
	real*8 Te, Damp,PGA,xMaxDisp
	CHARACTER(Len=180) :: path
	integer*8 :: num,I
      open(100,file="test.txt")
	read(100,*) path
	close(100)
      open(100,file="PGA.txt")
	read(100,*) PGA
	close(100)
	open(55,file=path)
	    if(EOF(55)/= .TRUE.)  then
              read(55,*)
              read(55,*)
              read(55,*) num    
          end if
	    i=0
		do while(EOF(55)/= .TRUE.)
	        i=i+1
			read(55,*) EQ(i,:)
	        EQ(i,2)=EQ(i,2)*PGA/100.d0
		end do
	close(55)

	open(66,file='xMAG.txt')
!	write(*,'(2A10)') "  T  ", "  Sa  "
	write(*,*) "Please wait for a moment"
	
      	pi=4.d0*atan(1.d0)
	do II=1, 600
		Te=real(II)*0.01d0  ! Period of system
	
		Damp=0.05d0; ! Damping ratio
		!call Average_Acce(EQ,Disp,Te, Damp,i,xMaxDisp)
	call Average_Acce(EQ,Disp,Te, Damp,i)
		xMaxDisp=maxval(abs(Disp))
 !        xMaxDisp=0
 !           do count=1,i
  !                if (xMaxDisp<abs(Disp(count))) then
!	            xMaxDisp=abs(Disp(count))
 !                 count=count+1
!	      end do
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
	integer*8 :: N
	real*8 EQ(100000,2)
	real*8 Disp(N)
	real*8 Te, Damp
	real*8 k, m, c, Omega,pi
	real*8 a1,a2,v1,v2,d1,d2
	real*8 dt
	integer I,J, STEP

	STEP=100 ! Number of substep for integration

	pi=4.d0*atan(1.d0);
      !xMaxDisp=0.d0;
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
	!   if (xMaxDisp<abs(Disp(I))) then
	 !     xMaxDisp=abs(Disp(I))
	  ! end if
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
	
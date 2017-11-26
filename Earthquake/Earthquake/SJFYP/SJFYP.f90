program pre_ear
    implicit none
    real*8 :: time,accel
    real*8 :: kesai,n1,n2,gama,tempmaxtime,tempmaxaccel,tempmintime,tempminaccel
    real*8 :: amax,tg,maxaccel,da1
    integer*8 :: i,num
    write(*,*) "Please wait......"
    kesai=0.05
    n1=0.02+(0.05-kesai)/(4.d0+32.d0*kesai)
    n2=1.d0+(0.05-kesai)/(0.08+1.6*kesai)
    gama=0.9+(0.05-kesai)/(0.3+6.d0*kesai)
    time=0
    accel=0
    open(100,file="SJFYP.txt")
    read(100,*) tg
    read(100,*) amax
    close(100)
    time=0
    accel=0.45*amax*9.8
    num=1
    
    open(200,file="SJFYPOUT.txt",position="append",status="replace",share="DENYNONE")  
        do while(time<=6.0)
        write(200,"(f15.3,f15.7)") time,accel     
        if(time<0.1) then
            time=0.1
            accel=n2*amax*9.8
            num=num+1
        else if(time >= 0.1 .AND. time < tg) then
            time=tg
            accel=n2*amax*9.8
            num=num+1
        else if(time >= tg .AND. time < tg+0.1) then
            time=time+0.001
            accel=(tg/time)**gama*n2*amax*9.8
            num=num+1
        else if(time >= tg+0.1 .AND. time < 5.d0*tg ) then
            time=time+0.01
            accel=(tg/time)**gama*n2*amax*9.8
            num=num+1
        else
            time=time+0.01
            accel=(n2*0.2**gama-n1*(time-5.d0*tg))*amax*9.8
            num=num+1
        end if
        end do
    close(200)
	open(200,file="max_minspe.txt")
	read(200,*) tempmaxtime
	read(200,*) tempmintime
	read(200,*) tempmaxaccel
	read(200,*) tempminaccel
	close(200)
    maxaccel=n2*amax*9.8
    da1=maxaccel
    i=2
    do while(floor(da1)<10)
        da1=da1*(10**i)
        i=i+2
    end do
    i=i-2
    da1=(floor(da1)+10)/real(10**i)
    open(200,file="max_minspe.txt",position="append",status="replace",share="DENYNONE")  

	if(tempmaxtime>=6.0) then
    write(200,"(F10.6)") tempmaxtime
	else
	write(200,"(A)") "6.0"
	end if

	if(tempmintime<=0.0) then
    write(200,"(F10.6)") tempmintime
	else
	write(200,"(A)") "0.0"
	end if
	
	if(tempmaxaccel>=da1) then
    write(200,"(F10.6)") tempmaxaccel
	else
    write(200,"(F10.6)") da1
	end if

	if(tempminaccel<=0.0) then
    write(200,"(F10.6)") tempminaccel
	else
	write(200,"(A)") "0.0"
	end if

    write(200,"(I3)") num
    close(200)
 
end program pre_ear
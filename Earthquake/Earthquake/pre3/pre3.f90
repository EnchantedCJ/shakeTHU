program pre_ear
    implicit none
    real*8 :: time,accel
    real*8 :: maxtime,mintime,dt,tempmaxtime,tempmintime
    real*8 :: maxaccel,minaccel,da1,da2,tempmaxaccel,tempminaccel
    integer*8 :: i,num
	open(200,file="max_minspe.txt")
	read(200,*) maxtime
	read(200,*) mintime
	read(200,*) maxaccel
	read(200,*) minaccel
	close(200)
    time=0
    accel=0
    tempmaxtime=maxtime
    tempmintime=mintime
    tempmaxaccel=maxaccel
    tempminaccel=maxaccel
	num=0
    open(100,file="xMAG.txt")
    do while(EOF(100) /= .true.)
		num=num+1
        read(100,*) time,accel
        if(time<mintime) then
            mintime=time
        end if
        if(time>maxtime) then
            maxtime=time
        end if
        if(accel<minaccel) then
            minaccel=accel
        end if
        if(accel>maxaccel) then
            maxaccel=accel
        end if   
    end do
    close(100)
    open(200,file="max_minspe.txt",position="append",status="replace",share="DENYNONE")
    da1=maxaccel
	if(maxaccel/=tempmaxaccel) then
    i=2
    do while(floor(da1)<10)
        da1=da1*(10**i)
        i=i+2
    end do
    i=i-2
    da1=(floor(da1)+10)/real(10**i)
	end if
    da2=minaccel
	if(minaccel/=tempminaccel) then
    i=2
    do while(floor(da2)>=-10 .and. da2/=0)
        da2=da2*(10**i)
        i=i+2
    end do
    i=i-2
    da2=(floor(da2))/real(10**i)
	end if
    !write(*,*) da2
    if(minaccel<0) then
    if(-da2>da1) then
    da1=-da2
    else
    da2=-da1
    end if
    end if
    write(200,"(F10.6)") maxtime
    write(200,"(F10.6)") mintime
    write(200,"(F10.6)") da1
    write(200,"(F10.6)") da2
	write(200,"(I6)") num
    close(200)
end program pre_ear
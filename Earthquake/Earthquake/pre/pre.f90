program pre_ear
    implicit none
    real*8 :: time,accel
    real*8 :: maxtime,mintime,dt,tempmaxtime,tempmintime
    real*8 :: maxaccel,minaccel,da1,da2,tempmaxaccel,tempminaccel
    integer*8 :: i,num
    CHARACTER(Len=180) :: path
	open(200,file="max_min.txt")
	read(200,*) maxtime
	read(200,*) mintime
	read(200,*) maxaccel
	read(200,*) minaccel
	read(200,*) num
	close(200)
    time=0
    accel=0
    tempmaxtime=maxtime
    tempmintime=mintime
    tempmaxaccel=maxaccel
    tempminaccel=maxaccel
    open(100,file="test.txt")
    read(100,*) path
    close(100)
    open(100,file=path)
    open(200,file="EQ01.txt",position="append",status="replace",share="DENYNONE")
    if(EOF(100)/= .TRUE.)  then
    read(100,*)
    read(100,*)
    read(100,*) num
    end if
    do while(EOF(100) /= .true.)
        read(100,*) time,accel
        write(200,"(f10.5,f10.5)")  time,accel
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
    close(200)
    open(200,file="max_min.txt",position="append",status="replace",share="DENYNONE")
	if(maxtime/=tempmaxtime) then
    dt=maxtime/10
    if(floor(dt)==dt) then
        dt=floor(dt)
    else
        dt=floor(dt)+1
    end if
	maxtime=dt*10
	end if
    da1=maxaccel
    da2=minaccel
    if(-da2>da1) then
    da1=-da2
    else
    da2=-da1
    end if
    write(200,"(F10.6)") maxtime
    write(200,"(F10.6)") mintime
    write(200,"(F10.6)") da1
    write(200,"(F10.6)") da2
    write(200,"(I8)")  num
    close(200)
end program pre_ear
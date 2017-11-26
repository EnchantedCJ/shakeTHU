program pre_ear
    implicit none
    real*8 :: time,accel
    real*8 :: maxtime,mintime,dt
    real*8 :: maxaccel,minaccel,da1,da2,angle
    integer*8 :: i,num,R,G,B
    CHARACTER(Len=180) :: path
    time=0
    accel=0
    maxtime=0
    mintime=0
    maxaccel=0
    minaccel=0
    open(300,file="angle.txt")
    read(300,*) angle
    close(300)
    open(100,file="test.txt")
    read(100,*) path
    close(100)
    open(100,file=path)
    open(200,file="EQX.txt",position="append",status="replace",share="DENYNONE")
    open(300,file="EQY.txt",position="append",status="replace",share="DENYNONE")
    if(EOF(100)/= .TRUE.)  then
    read(100,*)
    read(100,*)
    read(100,*) num
    end if
!	open(400,file="colorinfo.txt")
!	read(400,*) R
!	read(400,*) G
!	read(400,*) B
!	write(200,"(I6)") num
!	write(300,"(I6)") num
!	write(200,"(I3)") R
!	write(200,"(I3)") G
!	write(200,"(I3)") B
!	write(300,"(I3)") R
!	write(300,"(I3)") G
!	write(300,"(I3)") B
!	close(400)
    do while(EOF(100) /= .true.)
        read(100,*) time,accel
        write(200,"(f10.5,f10.5)")  time,accel*cos(angle/360*2*3.141592653)
        write(300,"(f10.5,f10.5)")  time,accel*sin(angle/360*2*3.141592653)
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
    close(300)
    
end program pre_ear
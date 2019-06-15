
drop database if exists lightning;
create database if not exists lightning;
use lightning;

------------------------------------------------------------------------

use lightning;

drop table if exists conductor;

create table conductor(
    cdtNumber int unsigned auto_increment,
    cdtSex char(1) default 'w' not null,
    cdtName nvarchar(100),
    cdtPhone char(11) not null,
    cdtScore int unsigned  default 0 not null,
    cdtPass nvarchar(100) not null,
    constraint pk_conductor primary key (cdtNumber),
    constraint unique_phone unique (cdtPhone)
);

insert  into conductor values(20171742,'m',"郭俊帅","15730343165",0,"qwert");

select * from conductor;

----------------------------------------------------------------------------------------

use lightning;

drop table if exists adminInfo;

create table adminInfo(
    adminNumber int unsigned not null auto_increment,
    adminName nvarchar(100),
    adminSex char(1) default 'w' not null,
    adminPhone char(11) not null,
    adminPass nvarchar(100) not null,
    constraint pk_admin primary key (adminNumber),
    constraint unique_adminPhone unique (adminPhone)
);

insert into adminInfo values(20171742, "郭俊帅", 'm','15730343165','qwert');

select * from adminInfo;

-------------------------------------------------------------------------

use lightning;

drop table if exists movie;

create table if not exists movie(
	movieName nvarchar(100) not null,
	movieDirector nvarchar(100),
	movieStar nvarchar(100),
	constraint pk_movie primary key (movieId),
	constraint unique_movieName unique (movieName)
);

insert into movie values ("霹雳战元史之动机风云","霹雳布袋戏","楼至韦陀");
insert into movie values ("霹雳惊鸿之刀剑春秋","霹雳布袋戏","意琦行/绮罗生");
insert into movie values ("霹雳侠影之轰动武林","霹雳布袋戏","三余无梦生");
insert into movie values ("霹雳侠影之轰定干戈","霹雳布袋戏","鷇音子");
insert into movie values ("霹雳侠影之轰掣天下","霹雳布袋戏","倦收天/原无乡");
insert into movie values ("霹雳侠影之轰霆剑海录","霹雳布袋戏","素还真");
insert into movie values ("霹雳开天记之创神篇","霹雳布袋戏","红冕七元");
insert into movie values ("霹雳开天记之创神篇下阙","霹雳布袋戏","赮毕钵罗/深海主宰");
insert into movie values ( "霹雳惊涛","霹雳布袋戏","意琦行");
insert into movie values ("霹雳魔封","霹雳布袋戏","青阳子");

select * from movie;

-- 增加一部电影
delimiter // 
create procedure addMovie(
	in mName nvarchar(100),  
	in mDirector nvarchar(100),  
	in mStar nvarchar(100) )  
begin
	insert into movie(movieName,movieDirector,movieStar) 
    values(mName,mDirector,mStar); 
end
 // 
delimiter ; 

-- 删除一部电影
delimiter //
create procedure removeMovie(
	in mName nvarchar(100))
begin
	delete from movie where movieName = mName;
end //
delimiter ;

-- 更新导演
delimiter //
create procedure modifyDirector(
	in mName nvarchar(100),
	in mDirector nvarchar(100))
begin
	update movie set movieDirector = mDirector 
	where movieName = mName; 
end//
delimiter ;


-- 修改主演
delimiter //
create procedure modifyStar(
	in mName nvarchar(100),
	in mStar nvarchar(100))
begin
	update movie set movieStar = mStar
	where movieName = mName;
end//
delimiter ;

---------------------------------------------------------------------------------

use lightning;

drop table if exists hall;

create table if not exists hall(
	hallId int unsigned not null,
	hallName nvarchar(100),
	hallRow int unsigned not null default 0,
	hallColumns int unsigned not null default 0,
	constraint pk_hall primary key (hallId)
);

insert into hall values (1,'琉璃仙境',30,50);
insert into hall values (2,'永旭之巅',30,50);
insert into hall values (3,'魔吞不动城',30,50);
insert into hall values (4,'指月山瀑',30,50);
insert into hall values (5,'画舫',30,50);
insert into hall values (6,'时间城',30,50);
insert into hall values (7,'红冕边城',30,50);
insert into hall values (8,'春宵幽梦楼',30,50);

delimiter //
create procedure addHall(
	in in_hallId int unsigned,
	in in_hallName nvarchar(100),
	in in_hallColumns int unsigned,
	in in_hallRow int unsigned)
begin
	insert into hall(hallId,hallName,hallColumns,hallRow) 
    values(in_hallId,in_hallName,in_hallColumns,in_hallRow);
end//
delimiter ;


delimiter //
create procedure removeHall(
	in del_hallId int unsigned)
begin
	delete from hall where hallId=del_hallId;
end//
delimiter ;


delimiter //
create procedure modifyHall(
	in mHallId int unsigned,
	in newHallName nvarchar(100))
begin 
	update hall set hallName=newHallName
	where hallId=mHallId;
end//
delimiter ;

select * from hall;

------------------------------------------------------------------------
use lightning;

drop table if exists label;

create table if not exists label(
	labelId nvarchar(15) not null,
	labelDsb nvarchar(100) not null,
	constraint pk_label primary key (labelId)
);

insert into label values("霹雳布袋戏","霹雳布袋戏");
insert into label values("金光布袋戏","金光布袋戏");
insert into label values("香港片","香港片");
insert into label values("动作片","动作片");
insert into label values("喜剧片","喜剧片");
insert into label values("恐怖片","恐怖片");
insert into label values("玄幻片","玄幻片");
insert into label values ("武侠片", "武侠片");

delimiter //
create procedure addLabel(
	in lid nvarchar(15),
	in ldsb nvarchar(100))
begin
 	insert into label(labelId,labelDsb) values (lid,ldsb);
end
//
delimiter ;

delimiter //
create procedure removeLabel(in lid nvarchar(15))
begin
	delete from label where labelId = lid;
end
//
delimiter ;

select * from label;

-----------------------------------------------------------------------------------

use lightning;

drop table if exists movieLabel;

create table if not exists movieLabel(
	movieName char(15),
	labelId nvarchar(15),
	constraint pk_movieLabel primary key (movieName, labelId),
	constraint fk_mvId foreign key (movieName) references movie(movieName),
	constraint fk_labId foreign key (labelId) references label(labelId)
);

insert into movieLabel values ("霹雳战元史之动机风云","霹雳布袋戏");
insert into movieLabel values ("霹雳惊鸿之刀剑春秋","霹雳布袋戏");
insert into movieLabel values ("霹雳侠影之轰动武林","霹雳布袋戏");
insert into movieLabel values ("霹雳侠影之轰定干戈","霹雳布袋戏");
insert into movieLabel values ("霹雳侠影之轰掣天下","霹雳布袋戏");
insert into movieLabel values ("霹雳侠影之轰霆剑海录","霹雳布袋戏");
insert into movieLabel values ("霹雳开天记之创神篇", "武侠片");
insert into movieLabel values ("霹雳开天记之创神篇下阙","玄幻片");

delimiter //
create procedure addMovieLabel(
	in mName nvarchar(100),
	in lId nvarchar(15))
begin
	insert into movieLabel(movieName,labelId) values(mName, lId);
end
//
delimiter ;

delimiter //
create procedure removeMovieLabel(
	in mName nvarchar(100),
	in lid nvarchar(15))
begin
	delete from movieLabel 
	where movieName = mName and labelId = lid;
end
//
delimiter ;

drop trigger if exists deleteMovieLabel1;
delimiter //
create trigger deleteMovieLabel1 
after delete on movie for each row
 begin
    delete from movieLabel where movieLabel.movieName = old.movieName;
 end//
delimiter ; 

drop trigger if exists deleteMovieLabel2;
delimiter //
create trigger deleteMovieLabel2
after delete on label for each row
 begin
    delete from movieLabel where movieLabel.labelId = old.labelId;
 end//
delimiter ;

select * from movieLabel;

-----------------------------------------------------------------------------------

use lightning;

drop table if exists timeLine;

create table if not exists timeLine(
    inTime time not null,
    playTime time not null,
    clearTime time not null,
    playDate date not null,
    timeNumber int unsigned not null,
    constraint pk_timeLine primary key (playDate,inTime,playTime,clearTime),
    constraint unique_timeNumber unique (timeNumber)
);

insert into timeLine values ("09:41:31","09:41:34","09:41:34","2019-06-07",1);
insert into timeLine values ("09:41:32","09:41:34","09:41:34","2019-06-07",2);
insert into timeLine values ("09:41:33","09:41:34","09:41:34","2019-06-07",3);
insert into timeLine values ("09:41:34","09:41:34","09:41:34","2019-06-07",4);
insert into timeLine values ("09:41:35","09:41:34","09:41:34","2019-06-07",5);
insert into timeLine values ("09:41:36","09:41:34","09:41:34","2019-06-07",6);
insert into timeLine values ("09:41:37","09:41:34","09:41:34","2019-06-07",7);
insert into timeLine values ("09:41:38","09:41:34","09:41:34","2019-06-07",8);

select * from timeLine;

delimiter //
create procedure addTimeLine(
    in iTime time,
    in pTime time,
    in cTime time,
    in pDate time,
    in tNumber int unsigned)
begin
    insert into timeLine(inTime,playTime,clearTime,playDate,timeNumber)
    values(iTime,pTime,cTime,pDate,tNumber);
end//
delimiter ;

delimiter //
create procedure removeTimeLine(in tNumber int unsigned)
begin
    delete from timeLine where timeNumber = tNumber;
end//
delimiter ;

delimiter //
create procedure removeDate(in oldDate date)
begin
    delete  from timeLine where playDate = oldDate;
end//
delimiter ;

delimiter //
create procedure modifyInTime(
    in tNumber int unsigned,
    in newTime time)
begin
    update timeLine set inTime = newTime 
    where timeNumber = tNumber;
end//
delimiter ;

delimiter //
create procedure modifyPlayTime(
    in tNumber int unsigned,
    in newTime time)
begin
    update timeLine set playTime = newTime
    where timeNumber = tNumber;
end//
delimiter ;

delimiter //
create procedure modifyClearTime(
    in tNumber int unsigned,
    in newTime time)
begin
    update timeLine set clearTime = newTime
    where timeNumber = tNumber;
end//
delimiter ;

------------------------------------------------------------------------

use lightning;

drop table if exists playState;

create table if not exists playState(
	stateId char(2) not null,
	stateDsb nvarchar(100) not null,
	constraint pk_playState primary key (stateId),
	constraint unique_stateDsb unique (stateDsb)
);


insert into playState values ("wa","等待状态(wait)");
insert into playState values ("sa","出售状态(sale)");
insert into playState values ("re","就绪状态(ready)");
insert into playState values ("pl","播放状态(play)");
insert into playState values ("co","播放完毕(complete)");

delimiter //
create procedure addPlayState(
	in stid char(2),
	in stdsb nvarchar(100))
begin
	insert into playState(stateId,stateDsb) values (stid, stdsb);
end
//
delimiter ;

delimiter //
create procedure removePlayState(in stid char(2))
begin
	delete from playState where stateId = stid;
end
//
delimiter ;

delimiter //
create procedure modifyPlayState(
	in stid char(2), 
	in stdsb nvarchar(100))
begin
	update playState set stateDsb = stdsb
	where stateId = stid;
end
//
delimiter ;

select * from playState;

-----------------------------------------------------------------------
use lightning;

drop table if exists stage;

create table if not exists stage(
    timeNumber int unsigned not null,
    hallId int unsigned not null,
    movieName nvarchar(100) not null,
    stateId char(2),
    price int unsigned not null,
    stageNumber int unsigned not null,
    constraint pk_stage primary key (timeNumber,hallId),
    constraint unique_stageNumber unique (stageNumber),
    constraint fk_timeNum foreign key (timeNumber) references timeLine(timeNumber),
    constraint fk_hallId foreign key (hallId) references hall(hallId),
    constraint fk_movieName foreign key (movieName) references movie(movieName),
    constraint fk_satId foreign key (stateId) references playState(stateId)
);

insert into stage values (1,1,"霹雳惊鸿之刀剑春秋","wa",80,1);
insert into stage values (2,2,"霹雳侠影之轰动武林","sa",70,2);
insert into stage values (3,3,"霹雳侠影之轰定干戈","re",90,3);
insert into stage values (4,4,"霹雳侠影之轰掣天下","pl",70,4);
insert into stage values (5,5,"霹雳侠影之轰霆剑海录","co",80,5);

-- 增加
delimiter //
create procedure addStage(
    in hId int unsigned,
    in tNumber int unsigned,
    in mName nvarchar(100),
    in p int(10),
    in sNumber int unsigned)
begin 
    insert into stage(hallId,timeNumber,movieName,staId,price,stageNumber)
    values(hId,tNumber,mName,'wa',p,sNumber);
end//
delimiter ;

-- 删除
delimiter //
create procedure removeStage(in sNumber int unsigned)
begin 
    delete from stage where stageNumber = sNumber;
end//
delimiter ;

--update price
delimiter //
create procedure modifyPrice(
    in sNumber int unsigned,
    in newPrice int unsigned)
begin 
    update stage set price=newPrice
    where stageNumber = sNumber;
end//
delimiter ;

--update movieId
delimiter //
create procedure modifyMovieName(
    in sNumber int unsigned,
    in mName  nvarchar(100))
begin 
    update stage set movieName = mName
    where stageNumber = sNumber;
end//
delimiter ;

--update stateId
delimiter //
create procedure modifyStateId(
    in sNumber int unsigned,
    in newStateId char(2))
begin 
    update stage set stateId = newStateId
    where stageNumber = sNumber;
end//
delimiter ;

delimiter //
create trigger removeStageTimeNumber 
after delete on timeLine for each row
begin
    delete from stage where stage.timeNumber = old.timeNumber;
end//
delimiter;

delimiter //
create trigger removeStageMovieName 
after delete on movie for each row
begin
    delete from stage where stage.movieName = old.movieName;
end//
delimiter;

delimiter //
create trigger removeStageHallId
after delete  on hall for each row
begin
    delete from stage where stage.hallId = old.hallId;
end//
delimiter;

delimiter //
create trigger removeStageStateId 
after delete on playState for each row
begin
    delete from stage where stage.stateId = old.stateId;
end//
delimiter;

select * from stage;

----------------------------------------------------------------------------------------------

use lightning;

drop view if exists stageView;

create view stageView(stageNumber,movieName,hallId, playDate, playTime,price,playState)
as
select stageNumber, movieName, hallId, playDate,playTime,price, stateDsb
from stage,playstate, timeline
where playstate.stateId = stage.stateId 
and timeline.timeNumber = stage.timeNumber;

select * from stageView;

----------------------------------------------------------------------------------------------

use lightning;

drop table if exists ticketState;

create table if not exists ticketState(
    tktStateId char(2) not null,
    tktStateDsb nvarchar(100) not null,
    constraint pk_ticketState primary key (tktStateId),
    constraint unique_tktStateDsb unique (tktStateDsb)
);

insert into ticketState values ("ye","已出售(yes)");
insert into ticketState values ("no","未出售(no)");
insert into ticketState values ("ba","退票(back)");

--插入过程
delimiter //
create procedure addTicketState(
    in tktStateId char(2),
    in tktStateDsb nvarchar(100))
begin 
    insert into ticketState(tktStateId,tktStateDsb) values(tktStateId,tktStateDsb);
end//
delimiter ;

--删除过程
delimiter //
create procedure removeTicketState(in mTktStateId char(2))
begin 
    delete from ticketState where tktStateId = mTktStateId;
end//
delimiter ;

select * from ticketState;

---------------------------------------------------------------------------------------------------

use lightning;

drop table if exists ticket;

create table if not exists ticket(
    stageNum int unsigned not null,
    sellState char(2) not null,
    hallRow int unsigned not null,
    hallColumn int unsigned not null,
    constraint pk_ticket primary key (stageNum, hallRow, hallColumn),
    constraint fk_satgeNum foreign key (stageNum) references stage(stageNumber),
    constraint fk_sellState foreign key (sellState) references ticketState(tktStateId)
);

insert into ticket values(1,"no",2,3);
insert into ticket values(2,"no",4,5);
insert into ticket values(3,"no",6,7);
insert into ticket values(4,"no",8,9);
insert into ticket values(5,"no",10,11);

delimiter //
create procedure addTicket(in sNum int unsigned)
begin
declare i int unsigned;
declare j int unsigned; 
declare hColumn int unsigned;
declare hRow int unsigned;
declare hid int unsigned;
set i=1;
set j=1;
set hColumn = 0;
set hRow = 0;
set hid = 0;

select stage.hallId into hid from stage 
where stage.stageNumber = sNum;
select hall.hallColumns,hall.hallRow 
into hColumn, hRow from hall
where hall.hallId = hid;

while i<= hRow do
    set j=1;
    while j<=hColumn do
        insert into ticket values(sNum,"no",i,j);
        set j = j + 1;
    end while;
    set i = i + 1;
end while;
end
//
delimiter ;

delimiter //
create procedure removeTicket(in sNum int unsigned)
begin
    delete from ticket where stageNum = sNum;
end
//
delimiter ;

delimiter //
create procedure sellTicket(
    in sNum int unsigned,
    in hColumn int unsigned,
    in hRow int unsigned,
    out flag int unsigned)
begin
declare _state char(2);
select sellState into _state from ticket 
where stageNum = sNum and hallColumn = hColumn and hallRow = hRow;

if _state  != "ye" then
    set flag = 1;
    update ticket set sellState = "ye"  where stageNum = sNum 
    and hallColumn = hColumn
    and hallRow = hRow;
else
set flag = 0;
end if;
end
//
delimiter ;

delimiter //
create procedure backTicket(
    in sNum int unsigned,
    in hColumn int unsigned,
    in hRow int unsigned,
    out flag int unsigned)
begin
declare _state char(2);
select sellState into _state from ticket where
stageNum = sNum and hallColumn = hColumn and hallRow = hRow;
if _state != "ye" then
set flag = 1;
update ticket set sellState = "ba" where 
stageNum = sNum and hallColumn = hColumn and hallRow = hRow;
else
set flag = 0;
end if;
end
//
delimiter ;

select * from ticket;

----------------------------------------------------------------------------

call addTicket(5);



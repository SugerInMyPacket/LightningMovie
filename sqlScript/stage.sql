use lightning;

drop table if exists stage;

create table if not exists stage(
    timeNum int unsigned not null,
    hallId int unsigned not null,
    movId char(15) ,
    staId char(2),
    stageNumber int unsigned not null,
    price int unsigned not null,
    constraint pk_stage primary key (timeNum,hallId),
    constraint unique_stageNumber unique (stageNumber),
    constraint fk_timeNum foreign key (timeNum) references timeLine(timeNumber),
    constraint fk_hallId foreign key (hallId) references hall(hallId),
    constraint fk_movId foreign key (movId) references movie(movieId),
    constraint fk_satId foreign key (staId) references playState(stateId)
);

insert into stage values (1,1,"1234567890abcdg","wa",1);
insert into stage values (2,2,"1234567890abcdh","sa",2);
insert into stage values (3,3,"1234567890abcdi","re",3);
insert into stage values (4,4,"1234567890abcdj","pl",4);
insert into stage values (5,5,"1234567890abcdk","co",5);

select * from stage;

-- 增加
delimiter //
create procedure addStage(
    in hallId int unsigned,
    in movId char(15),
    in price int(10),
    in stageNumber int unsigned,
    in staId char(2),
    in timeNum int unsigned)
begin 
    insert into stage values(hallId,movId,price,stageNumber,staId,timeNum);
end//
delimiter ;

-- 删除
delimiter //
create procedure removeStage(
    in mHallId int unsigned,
    in mTimeNum int unsigned)
begin 
    delete from where hallId=mHallId and timeNum=mTimeNum;
end//
delimiter ;

--更改过程
--update price
delimiter //
create procedure modifyPrice(
    in mhallId int unsigned,
    in mTimeNum int unsigned,
    in newPrice int unsigned
)
begin 
    update stage set price=newPrice
    where hallId=mHallId and timeNum=mTimeNum;
end//
delimiter ;

--update movieId
delimiter //
create procedure modifyMovId(
    in mhallId int unsigned,
    in mTimeNum int unsigned,
    int mMovName  nvarchar(100))
begin 
    declare id char(15);
    select movieId into id from movie where movieName = mMovName;
    update stage set movId=id
    where hallId=mHallId and timeNum=mTimeNum;
end//
delimiter ;

--update stateId
delimiter //
create procedure modifyStateId(
    in mhallId int unsigned,
    in mTimeNum int unsigned,
    in newStateId char(2)
)
begin 
    update stage set staId=newStateId
    where hallId=mHallId and timeNum=mTimeNum;
end//
delimiter ;




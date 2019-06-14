  
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

insert into ticket values(1,100,"no",2,3);
insert into ticket values(2,100,"no",4,5);
insert into ticket values(3,100,"no",6,7);
insert into ticket values(4,100,"no",8,9);
insert into ticket values(5,100,"no",10,11);

select * from ticket;


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
    in hRow int unsigned)
begin
update ticket set sellState = "ye" 
where stageNum = sNum 
and hallColumn = hColumn
and hallRow = hRow;
end
//
delimiter ;

delimiter //
create procedure backTicket(
    in sNum int unsigned,
    in hColumn int unsigned,
    in hRow int unsigned)
begin
update ticket set sellState = "ba" 
where stageNum = sNum 
and hallColumn = hColumn
and hallRow = hRow;
end
//
delimiter ;


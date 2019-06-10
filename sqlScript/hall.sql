use lightning;

drop table if exists hall;

create table if not exists hall(
	hallId int unsigned not null,
	hallName nvarchar(100),
	hallRow int unsigned not null default 0,
	hallColumns int unsigned not null default 0,
	constraint pk_hall primary key (hallId)
);

insert into hall values (1,'琉璃仙境',100,100);
insert into hall values (2,'永旭之巅',100,100);
insert into hall values (3,'魔吞不动城',100,100);
insert into hall values (4,'指月山瀑',100,100);
insert into hall values (5,'画舫',100,100);
insert into hall values (6,'时间城',100,100);
insert into hall values (7,'红冕边城',100,100);
insert into hall values (8,'春宵幽梦楼',100,100);

select * from hall;


delimiter //
create procedure addHall(
	in in_hallId int unsigned,
	in in_hallName nvarchar(100),
	in in_hallColumns int unsigned,
	in in_hallRow int unsigned)
begin
	insert into hall values(in_hallId,in_hallName,in_hallColumns,in_hallRow);
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


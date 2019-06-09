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

select * from playState;


delimiter //
create procedure addPlayState(
	in stid char(2),
	in stdsb nvarchar(100))
begin
	insert into playState values (stid, stdsb);
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


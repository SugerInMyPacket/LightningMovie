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

delimiter //
create procedure     

delimiter ;
 
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



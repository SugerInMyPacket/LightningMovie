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

select * from label;


delimiter //
create procedure addLabel(
	in lid nvarchar(15),
	in ldsb nvarchar(100))
begin
 	insert into label values (lid,ldsb);
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


delimiter //
create procedure modifylabel(
	in lid nvarchar(15), in ldsb nvarchar(100))
begin
	update label set labelDsb = ldsb where labelId = lid;
end
//
delimiter ;

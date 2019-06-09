
use lightning;

drop view if exists stageView;

create view stageView(stageNumber,price,hallId, movieName,playState, playDate, playTime )
as
select stageNumber, price, hallId, movieName, stateDsb, playDate,playTime
from stage,movie,playstate,timeline
where movieId  = movId and stateId  = staId and timeNumber = timeNum;

select * from stageView;


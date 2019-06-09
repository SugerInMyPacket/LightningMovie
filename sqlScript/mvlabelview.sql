
use lightning;

drop view if exists movieLabelView;

create view movieLabelView(movieName, movieLabel)
as
select movieName, labId 
from movie, movieLabel
where movId = movieId
order by movieName asc;

select * from movieLabelView;
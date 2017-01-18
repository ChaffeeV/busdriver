# busdriver

Карту интересуещего вас региона можно скачать на https://overpass-turbo.eu/

1. Выбираем нужную область на карте (масштаб, не больше 1/150м)
2. В окне слева вводим:

[out:json][timeout:25];
// gather results
(
  // query part for: “building”
  node["building"]({{bbox}});
  way["building"]({{bbox}});
  relation["building"]({{bbox}});
);
// print results
out body;
>;
out skel qt;

3. Нажимаем "Старт", затем "Экспорт"->"Данные"->"как geoJSON"
4. Сохраняем как build.geojson
5. В окне слева вводим:

[out:json][timeout:25];
// gather results
(
  // query part for: “highway=* and type!=multipolygon and area!=*”
  way["highway"]["type"!="multipolygon"]["area"!~".*"]({{bbox}});
  relation["highway"]["type"!="multipolygon"]["area"!~".*"]({{bbox}});
);
// print results
out body;
>;
out skel qt;

6. Повторяем пункт 3 и 4, только сохраняем как high.geojson
7. build.geojson и high.geojson перемещаем в папку Data.
8. Запускаем mapmaker.

Структура Map:
1-ая строчка - общее количество элементов (зданий и дорог)
Далее, здание задается как 2 N, где N - количество точек (первая считается дважды, так как она и последняя), а дорога задается как 1 N, где N - количество точек в дороге.
После этого, 1 -ая строчка - координаты по оси x, 2-ая - по оси y.


bisect f xl xu
    | f xl * f xr < 0 = bisect f xl xr
    | f xr * f xu < 0 = bisect f xr xu
    where
        xr = (xl + xu) / 2 



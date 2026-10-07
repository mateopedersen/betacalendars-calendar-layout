# Civil-date model

`civil_date` stores a proleptic Gregorian year, month, and day. Years 1 through 9999 are supported. `valid_date` should be used for untrusted inputs. Weekday indices are Monday-first, where Monday is zero. Gregorian arithmetic is integer-only and independent of time zones and daylight-saving transitions.

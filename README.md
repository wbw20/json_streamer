APOLLO
=============

by Carbon Origins

This is the operating system for Apollo.  The following api can be used over USB.

## API

```code
{
  "command": "data"
}
```

```code
{
  "command": "data",
  "format": "csv"
}
```


```code
{
  "command": "pinMode",
  "pin": 13,
  "mode": 1
}
```

```code
{
  "command": "digitalWrite",
  "pin": 13,
  "mode": 0
}
```

## Expirimental API

```code
{
  "command": "log",
  "sensors": [
    "temperature",
    "humidity",
    "pressure"
  ],
  "frequency": 100
}
```

```code
{
  "command": "pressure"
}
```

# Zenoh Differences

## Description

This document documents instances where Zenoh nodes differ from the montblanc topology. If those differences are a bug, then this is what basically amounts to an issue tracker.

This list is **non-exhaustive**, they just include the ones I've come across.

- `geneva` and `arequipa` wrong type on `/arkansas`
  
  - `Float32` needs to be changed to `String`

- `osaka` wrong subscribes
  
  - `/parana` and `/delhi` should be dropped
  
  - `/columbia` should be subscribed

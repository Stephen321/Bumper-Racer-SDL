@echo off
plink -i "C:\Users\Stephen\private_keys\ArgoTeamD.pem" ubuntu@34.251.220.117 "cd socketServer/; ./start.sh"


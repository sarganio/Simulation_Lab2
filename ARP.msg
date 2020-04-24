// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

//
// TODO generated message class
//
packet ARP {
    unsigned short opcode;
    unsigned char srcMAC[8];//e.g. 01-23-45-67-89-AB-CD-EF
    unsigned char srcIP[4];//e.g. 172.168.32.1
    unsigned char trgMAC[8];//e.g. 01-23-45-67-89-AB-CD-EF
    unsigned char trgIP[4];//e.g. 172.168.32.1
}


/*     List events is a generator of events
*      Copyright (c) 2019 Marlon W. Santos <marlon.santos.santos@icen.ufpa.br>
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>5.
*
*/


package org.contikios.cooja.dialogs;

  //Permite salvar em arquivo os outputs do System.out.println
import java.io.*;
import java.io.File;
import java.io.FileOutputStream;
import java.io.PrintStream;
import java.util.Random;

public class ListEvents{

  private int time,count_motes;
  private int id_mote;

  private double[] coordenateX;
	private double[] coordenateY;
  private double[] coordenateZ;
  private int startX,endX,startY,endY,startZ,endZ;

  public void set_time(int time){
    this.time=time;
  }

  public int get_time(){
    return time;
  }

  public void set_id_mote(int id_mote){
    this.id_mote=id_mote;
  }
  
  public int get_id_mote(){
    return id_mote;
  }

  public void set_count_motes(int count_motes){
    this.count_motes=count_motes;
  }
  
  public int get_count_motes(){
    return count_motes;
  }

  public void set_coordenates(double coordenateX,double coordenateY,double coordenateZ){
    if(get_id_mote()==0){
      int n = get_count_motes();
      this.coordenateX = new double[n];
      this.coordenateY = new double[n];
      this.coordenateZ = new double[n];
    }

    int i=get_id_mote();
    this.coordenateX[i]=coordenateX;
    this.coordenateY[i]=coordenateY;
    this.coordenateZ[i]=coordenateZ;
  }

  public void set_area(int startX,int endX,int startY,int endY,int startZ,int endZ){
    this.startX=startX;
    this.endX=endX;
    this.startY=startY;
    this.endY=endY;
    this.startZ=startZ;
    this.endZ=endZ;
  }

  public int get_startX(){
    return startX;
  }

  public int get_endX(){
    return endX;
  }

  public int get_startY(){
    return startY;
  }

  public int get_endY(){
    return endY;
  }

  public int get_startZ(){
    return startZ;
  }

  public int get_endZ(){
    return endZ;
  }

  public void save_coordenate(){

      try{
        System.setOut(new PrintStream(new FileOutputStream("coordenadas.h",false)));
        
        System.out.println("float motes_coordenates["+get_count_motes()+"][3]={");
        read_coordenates();
        System.out.println("};");
      }catch(FileNotFoundException ex){System.out.println("Erro ao criar arquivo!");};
  }     

  public void read_coordenates(){
    for (int i=0;i<get_count_motes();i++){  
      if(i<get_count_motes()-1){

        System.out.println("{"+get_coordX(i)+","+get_coordY(i)+","+get_coordZ(i)+"},");
      }else{
        System.out.println("{"+get_coordX(i)+","+get_coordY(i)+","+get_coordZ(i)+"}");
      }
    }
  }

  public double get_coordX(int i){
    return coordenateX[i];
  }

  public double get_coordY(int i){
    return coordenateY[i];
  }

  public double get_coordZ(int i){
    return coordenateZ[i];
  }

  public void save_events(){
    try{
      System.setOut(new PrintStream(new FileOutputStream("events.h", false)));
      System.out.println("int coordenates=3;");
      System.out.println("float events_coordenates["+get_time()+"][3]={");
      generate_events();
      System.out.println("};\n");

    }catch(FileNotFoundException ex){System.out.println("Erro ao criar arquivo!");}; 
  }

  public void generate_events(){
    
    for(int i=0;i<get_time();i++){
      if(i<get_time()-1){
        System.out.println("{"+random_coordX()+","+random_coordY()+","+random_coordZ()+"},");
      }else{
        System.out.println("{"+random_coordX()+","+random_coordY()+","+random_coordZ()+"}");
      }
    }
  }

  public double random_coordX(){
    return Math.random()*(get_endX()-get_startX())+get_startX();
  }

  public double random_coordY(){
    return Math.random()*(get_endY()-get_startY())+get_startY();
  }

  public double random_coordZ(){
    return Math.random()*(get_endZ()-get_startZ())+get_startZ();
  }
}

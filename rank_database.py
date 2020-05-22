import pandas as pd
import sqlite3

# stock ticker symbol
url1 = "https://api.thingspeak.com/channels/73*****/fields/6.csv?api_key=*******"
url2 = "https://api.thingspeak.com/channels/738300/fields/7.csv?api_key=********"


# import data with pandas
data = pd.read_csv(url1)
data1 = pd.read_csv(url2)
req_data = str(max(data['field6']))
req_data1 = str(max(data1['field7']))
reqq_data = [req_data,req_data1]
appliance_name = ["Shakti Pumps","Bhakti Pumps"]

con=None
cursor=None
for i in range(2):
    try:
                
        con=sqlite3.connect("your_database_name.db")
        sql = "insert into appliances(Appliance_Name,Power) values('%s','%s')"
        args = (appliance_name[i],reqq_data[i])
        cursor = con.cursor()
        cursor.execute(sql % args)
        con.commit()
        print(cursor.rowcount , "Records inserted")
    except Exception as e:
        print("Add issue",e)
        con.rollback()
    finally:
        if cursor is not None:
                cursor.close()
        if con is not None:
                con.close();

while True:
    opt = int(input("1 View Record,2 Exit"))
    if opt == 1:
        con = None
        cursor = None
        try:
                        con = sqlite3.connect("your_database_name.db")
                        sql = "select distinct Appliance_Name,Power from appliances order by (Power) asc"
                        cursor = con.cursor()
                        cursor.execute(sql)
                        data = cursor.fetchall()
                        df = pd.read_sql_query(sql, con)
                        #df = pd.DataFrame(df1, columns= ['Appliance Name','Power(Watt)'])
                        export_excel = df.to_excel (r'mention the directory to save the excel database here\Appliance_Rankings.xlsx', index = None, header=True)
                        for d in data:
                                
                                print(d[0],d[1])
        except Exception as e:
                        print("select issue",e)
        finally:
                        if cursor is not None:
                                cursor.close()
                        if con is not None:
                                con.close();
    elif opt == 2:
                break
    else:
                print("Invalid Option")
    

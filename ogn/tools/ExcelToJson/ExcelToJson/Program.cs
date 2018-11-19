using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data;
using System.Data.OleDb;
using System.IO;
using LitJson;


public class StructData
{
    public List<bool> number = new List<bool>();
    public List<string> fileds = new List<string>();
}

namespace ExcelToJson
{

    class Program
    {
        static void Main(string[] args)
        {
            string path = System.Environment.CurrentDirectory;
            //var files = Directory.GetFiles(path, "*.txt");

            //foreach (var file in files)
            //    Console.WriteLine(file);

            //第二种方法;
            DirectoryInfo folder = new DirectoryInfo(path);

            foreach (FileInfo file in folder.GetFiles("*.xlsx"))
            {
                if (((int)file.Attributes & (int)FileAttributes.Hidden) != 0)
                    continue;
                ExcelToJson(file.FullName);
                Console.WriteLine(file.FullName);
            }
            Console.Write("Press any key to continue . . . ");
            Console.ReadKey(true);

        }
        static void ExcelToJson(string xlsx)
        {
            List<string> tableNames = ExcelHelper.GetExcelSheetNames(xlsx);
            //var json = new JObject();
            foreach (string tableName in tableNames)
            {
                if (tableName != "Shelve" && tableName != "OffShelve")
                    continue;

                JsonData json = new JsonData();
                JsonData table = new JsonData();
                StructData data = new StructData();
                DataTable dataTable = ExcelHelper.GetExcelContent(xlsx, tableName, data);
                if (dataTable.Rows.Count <= 0)
                    continue;

                foreach (DataRow dataRow in dataTable.Rows)
                {
                    JsonData row = new JsonData();
                    int idx = 0;
                    foreach (DataColumn column in dataTable.Columns)
                    {
                        bool isValid = false;
                        for (int i = 0; i < dataRow.ItemArray.Length; ++i)
                        {
                            if (dataRow.ItemArray[i].ToString().Length > 0)
                            {
                                isValid = true;
                                break;
                            }
                        }
                        if (!isValid)
                            continue;

                        JsonData jtoken;

                        string s = dataRow.ItemArray[idx] == null ? "" : dataRow.ItemArray[idx].ToString();
                        if (data.number[idx])
                        {
                            if (s.IndexOf('.') != -1)
                            {
                                jtoken = new JsonData(float.Parse(s));
                            }
                            else
                            {
                                jtoken = new JsonData(s == "" ? 0 : int.Parse(s));
                            }
                        }
                        else
                        {
                            jtoken = new JsonData(s);
                        }
                        row[data.fileds[idx]] = jtoken;
                        idx++;
                    }
                    table.Add(row);
                }
                json["config"] = table;

                //Console.WriteLine(json.ToString());

                string jsonArray = json.ToJson();
                byte[] jsonBytes = System.Text.Encoding.Default.GetBytes(jsonArray);
                FileStream fs = new FileStream(tableName + ".json", FileMode.Create);
                fs.Write(jsonBytes, 0, jsonBytes.Length);
                fs.Close();
                Console.WriteLine(tableName + ".json");
            }
            //Console.WriteLine(json.ToString(Formatting.None));
        }
    }
}


public class ExcelHelper
{
    public static DataTable GetExcelContent(String filePath, string sheetName, StructData data)
    {
        if (sheetName == "_xlnm#_FilterDatabase")
            return null;
        DataSet dateSet = new DataSet();
        String connectionString = String.Format("Provider=Microsoft.ACE.OLEDB.12.0;Data Source={0};Extended Properties='Excel 12.0;HDR=NO;IMEX=1;'", filePath);
        String commandString = string.Format("SELECT * FROM [{0}$]", sheetName);
        using (OleDbConnection connection = new OleDbConnection(connectionString))
        {
            connection.Open();
            using (OleDbCommand command = new OleDbCommand(commandString, connection))
            {
                OleDbCommand objCmd = new OleDbCommand(commandString, connection);
                OleDbDataAdapter myData = new OleDbDataAdapter(commandString, connection);
                myData.Fill(dateSet, sheetName);
                DataTable table = dateSet.Tables[sheetName];
                table.Rows.RemoveAt(0);
                if (table.Rows.Count <= 0)
                    return table;
                table.Columns.RemoveAt(0);
                for (int i = 0; i < table.Rows[0].ItemArray.Length; i++)
                {
                    var cloumnName = table.Rows[0].ItemArray[i].ToString() == "真" ? true : false;
                    data.number.Add(cloumnName);
                }
                table.Rows.RemoveAt(0);
                if (table.Rows.Count <= 0)
                    return table;

                for (int i = 0; i < table.Rows[0].ItemArray.Length; i++)
                {
                    var cloumnName = table.Rows[0].ItemArray[i].ToString();
                    data.fileds.Add(cloumnName);
                }
                table.Rows.RemoveAt(0);
                return table;
            }
        }
    }

    public static List<string> GetExcelSheetNames(string filePath)
    {
        OleDbConnection connection = null;
        System.Data.DataTable dt = null;
        try
        {
            String connectionString = String.Format("Provider=Microsoft.ACE.OLEDB.12.0;Data Source={0};Extended Properties='Excel 12.0;HDR=YES;IMEX=1;'", filePath);
            connection = new OleDbConnection(connectionString);
            connection.Open();
            dt = connection.GetOleDbSchemaTable(OleDbSchemaGuid.Tables, null);

            if (dt == null)
            {
                return new List<string>();
            }

            String[] excelSheets = new String[dt.Rows.Count];
            int i = 0;
            foreach (DataRow row in dt.Rows)
            {
                excelSheets[i] = row["TABLE_NAME"].ToString().Split('$')[0];
                i++;
            }
            return excelSheets.Distinct().ToList();
        }
        catch (Exception ex)
        {
            return new List<string>();
        }
        finally
        {
            if (connection != null)
            {
                connection.Close();
                connection.Dispose();
            }
            if (dt != null)
            {
                dt.Dispose();
            }
        }
    }

}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Data.SQLite;

namespace CreateEncryptedDB
{
    class Program
    {
        const string DB_FILE_PATH = "test.db";
        const string PASSWORD = "lock";

        static void Main(string[] args)
        {
            try
            {
                if (System.IO.File.Exists(DB_FILE_PATH))
                {
                    System.IO.File.Delete(DB_FILE_PATH);
                }

                SQLiteConnectionStringBuilder cnnstrBld =
                    new SQLiteConnectionStringBuilder { DataSource = DB_FILE_PATH, Password = PASSWORD };
                using (SQLiteConnection cnn = new SQLiteConnection(cnnstrBld.ToString()))
                {
                    cnn.Open();
                    SQLiteCommand cmd = new SQLiteCommand(cnn);
                    cmd.CommandText = "create table if not exists TEST(TextCol TEXT NOT NULL)";
                    cmd.ExecuteNonQuery();

                    cmd.CommandText = "insert into TEST(TextCol) values('hello sqlite')";
                    cmd.ExecuteNonQuery();

                    cmd.CommandText = "select TextCol from TEST";
                    var insertedText = cmd.ExecuteScalar().ToString();
                    Console.WriteLine("Inserted text:{0}", insertedText);
                    Console.ReadKey();
                }
            }
            catch(Exception ex)
            {
                Console.WriteLine(ex.Message + Environment.NewLine + ex.StackTrace);
            }
        }
    }
}

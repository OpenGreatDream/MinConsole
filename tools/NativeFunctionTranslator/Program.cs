﻿using System;
using System.IO;
using System.Collections.Generic;
using System.Text;

namespace NativeFunctionTranslator
{
    struct Param
    {

    }

    class Program
    {
        public static List<string> GetFileListWithExtend(DirectoryInfo directory, string pattern)
        {
            List<string> pathList = new List<string>();
            string result = string.Empty;
            if (directory.Exists || pattern.Trim() != string.Empty)
            {
                foreach (FileInfo info in directory.GetFiles(pattern))
                {
                    result = info.FullName.ToString();
                    pathList.Add(result);
                }
            }
            return pathList;
        }

        static void Main(string[] args)
        {
            DirectoryInfo directory = new DirectoryInfo(Environment.CurrentDirectory);

            string MinConsoleFolder = directory.Parent.Parent.Parent.Parent.Parent.ToString();

            //try to read
            string targetFolder = Path.Combine(MinConsoleFolder, "src\\MinConsoleNative");
            //try to write
            string targetFile = Path.Combine(MinConsoleFolder, "src\\MinConsole\\MinConsoleNativeFuncs.cs");

            List<string> headFiles = GetFileListWithExtend(new DirectoryInfo(targetFolder), "*.h");

            List<string> targetFileLines = new List<string>();
            targetFileLines.Add("using System;");
            targetFileLines.Add("using System.Runtime.InteropServices;");
            targetFileLines.Add("");
            targetFileLines.Add("namespace MinConsole");
            targetFileLines.Add("{");
            targetFileLines.Add("    internal class MinConsoleNativeFuncs");
            targetFileLines.Add("    {");
            targetFileLines.Add("        //>>>insert_here<<<");
            targetFileLines.Add("    }");
            targetFileLines.Add("}");

            int insertLineNumber = 0;

            const string EXPORT_FUNC = "EXPORT_FUNC";
            const string INSERT_HERE = "//>>>insert_here<<<";
            //In C# file:
            const string EXPORT_FUNC_DLLIMPORT =
                "[DllImport(\"MinConsoleNative.dll\", CallingConvention = CallingConvention.StdCall, SetLastError = true, CharSet = CharSet.Unicode)]";
            const string EXPORT_FUNC_RETURN_TYPE = "public extern static bool";
            const int indent = 8;
            string indentLine = string.Empty;
            StringBuilder builder = new StringBuilder();
            for (int i = 0; i < indent; i++)
            {
                builder.Append(" ");
            }
            indentLine = builder.ToString();

            List<string> nativeMethodDeclaration = new List<string>();

            foreach (string headFile in headFiles)
            {
                string text = File.ReadAllText(headFile);
                string[] lines = text.Split(Environment.NewLine);

                foreach (string line in lines)
                {
                    string _line = line.Trim();

                    if (_line.IndexOf(EXPORT_FUNC) != -1)
                    {
                        bool equal = true;
                        for (int i = 0; i < EXPORT_FUNC.Length; i++)
                        {
                            if (_line[i] != EXPORT_FUNC[i])
                            {
                                equal = false;
                                break;
                            }
                        }

                        if (equal && !nativeMethodDeclaration.Contains(_line))
                        {
                            nativeMethodDeclaration.Add(_line);
                        }
                    }
                }
            }

            for (int i = 0; i < targetFileLines.Count; i++)
            {
                string fileLine = targetFileLines[i];

                string _line = fileLine.Trim();

                if (_line.IndexOf(INSERT_HERE) != -1)
                {
                    bool equal = true;
                    for (int j = 0; j < INSERT_HERE.Length; j++)
                    {
                        if (_line[j] != INSERT_HERE[j])
                        {
                            equal = false;
                            break;
                        }
                    }
                    if (equal)
                    {
                        insertLineNumber = i + 1;
                    }
                }
            }

            List<string> readyToWrite = new List<string>();

            foreach (string item in nativeMethodDeclaration)
            {
                string newLine = item.Replace(EXPORT_FUNC, EXPORT_FUNC_RETURN_TYPE);

                readyToWrite.Add(indentLine + EXPORT_FUNC_DLLIMPORT);
                readyToWrite.Add(indentLine + newLine);
                readyToWrite.Add("");
            }

            List<string> finalLines = new List<string>();
            //add head
            for (int i = 0; i < insertLineNumber; i++)
            {
                finalLines.Add(targetFileLines[i]);
            }
            //add content
            finalLines.AddRange(readyToWrite);
            //add tail
            for (int i = insertLineNumber; i < targetFileLines.Count; i++)
            {
                finalLines.Add(targetFileLines[i]);
            }

            Console.WriteLine("Hello World!");
            Console.ReadKey();
        }
    }
}

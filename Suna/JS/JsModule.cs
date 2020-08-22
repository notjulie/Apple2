using System;
using System.Collections;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Jint;

namespace Suna.JS
{
   /// <summary>
   /// Represents a JavaScript module
   /// </summary>
   public class JsModule
   {
      private enum Error
      {
         ExpressionNotArray
      }

      private Engine jsEngine = new Jint.Engine();

      /// <summary>
      /// Evaluates a string as a byte array
      /// </summary>
      /// <param name="jsString"></param>
      /// <returns></returns>
      public byte[] EvaluateByteArray(string jsString)
      {
         IEnumerable collection = jsEngine.Execute(jsString).GetCompletionValue().ToObject() as IEnumerable;
         if (collection == null)
            throw new CompileException(Error.ExpressionNotArray);

         List<byte> result = new List<byte>();
         foreach (object o in collection)
            result.Add(Convert.ToByte(o, CultureInfo.InvariantCulture));
         return result.ToArray();
      }

      /// <summary>
      /// Executes a JavaScript string
      /// </summary>
      /// <param name="jsString"></param>
      public void Execute(string jsString)
      {
         jsEngine.Execute(jsString);
      }

      /// <summary>
      /// Returns true of the given name is a JavaScript function
      /// </summary>
      /// <param name="name"></param>
      /// <returns></returns>
      public bool IsFunctionName(string name)
      {
         return jsEngine.Execute(name + " instanceof Function").GetCompletionValue().AsBoolean();
      }
   }
}

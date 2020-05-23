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
   class JsModule
   {
      private enum Error
      {
         ExpressionNotArray
      }

      private Engine jsEngine = new Jint.Engine();

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

      public void Execute(string jsString)
      {
         jsEngine.Execute(jsString);
      }

      public bool IsFunctionName(string name)
      {
         return jsEngine.Execute(name + " instanceof Function").GetCompletionValue().AsBoolean();
      }
   }
}

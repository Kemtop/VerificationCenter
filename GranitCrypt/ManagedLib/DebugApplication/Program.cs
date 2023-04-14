using LizardEncryptLib.CipherStore;
using LizardEncryptLib.LowLayerFunction;

Console.WriteLine(StoreNames.LocalStoreEncryptName);
Console.WriteLine(StoreNames.LocalStoreDecryptName);

ILowLayerFunctions adapter = new NativeAdapter();

adapter.LoadSecretKey("SecretKey.key");
Console.WriteLine(adapter.CheckPasswordForContainer("1237454545"));
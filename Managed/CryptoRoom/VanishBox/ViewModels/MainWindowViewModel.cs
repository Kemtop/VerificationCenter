using ReactiveUI;
using System.Collections.ObjectModel;
using System.Reactive;
using System.Reactive.Linq;
using System.Threading.Tasks;

namespace VanishBox.ViewModels
{
    public class MainWindowViewModel : ViewModelBase
    {
        public ObservableCollection<string> FilesPaths { get; private set; } = new();

        /// <summary>
        /// Выбрать файлы которые требуется зашифровать/расшифровать.
        /// </summary>
        public ReactiveCommand<Unit, Unit> SelectFilesCommand { get; }

        /// <summary>
        /// Сбросить настройки программы.
        /// </summary>
        public ReactiveCommand<Unit, Unit> ResetSettingsCommand { get; }

        /// <summary>
        /// Сгенерировать ключ.
        /// </summary>
        public ReactiveCommand<Unit, Unit> GeneratedKeyCommand { get; }

        /// <summary>
        /// Ввод пароля и пути сохранения секретного ключа.
        /// </summary>
        public Interaction<PasswordInputWindowViewModel, UserKeyViewModel?> ShowKeyParamDialog { get; }

        /// <summary>
        /// Сбросить настройки программы.
        /// </summary>
        public Interaction<string, bool> ResetSettingsDialog { get; }

        /// <summary>
        /// Выбрать файлы которые требуется зашифровать/расшифровать.
        /// </summary>
        public Interaction<object, string[]?> SelectFilesDialog { get; }

        private string[] _selectedFiles { get; set; }
        
        public MainWindowViewModel()
        {
            SelectFilesDialog = new Interaction<object, string[]?>();
            ResetSettingsDialog = new Interaction<string, bool>();
            ShowKeyParamDialog = new Interaction<PasswordInputWindowViewModel, UserKeyViewModel?>();

            SelectFilesCommand = ReactiveCommand.CreateFromTask(async () =>
            {
               await SelectFiles();
            });

            ResetSettingsCommand = ReactiveCommand.CreateFromTask(async () =>
            {
                await ResetSettings();
            });

            GeneratedKeyCommand = ReactiveCommand.CreateFromTask(async () =>
            {
                await GeneratedKey();
            });
        }

        /// <summary>
        /// Выбрать файлы которые требуется зашифровать/расшифровать.
        /// </summary>
        /// <returns></returns>
        public async Task SelectFiles()
        {
            _selectedFiles = await SelectFilesDialog.Handle(new object());

            FilesPaths.Clear();
            foreach (var item in _selectedFiles)
            {
                FilesPaths.Add(item);
            }
        }

        /// <summary>
        /// Сбросить настройки программы.
        /// </summary>
        /// <returns></returns>
        public async Task ResetSettings()
        {
            var result = await ResetSettingsDialog.Handle("Вы действительно хотите сбросить настройки?");
            if (result == false) return;

            FilesPaths.Clear();
            FilesPaths.Add("Настройки сброшены.");
        }

        /// <summary>
        /// Сгенерировать ключ.
        /// </summary>
        /// <returns></returns>
        public async Task GeneratedKey()
        {
            var keyParam = new PasswordInputWindowViewModel();
            var result = await ShowKeyParamDialog.Handle(keyParam);
            if (result == null) return;

            FilesPaths.Clear(); //Тест.
            FilesPaths.Add(result.Password);
            FilesPaths.Add(result.Path);
        }
    }
}
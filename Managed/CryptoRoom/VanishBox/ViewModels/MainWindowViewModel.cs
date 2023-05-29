using ReactiveUI;
using System.Collections.ObjectModel;
using System.Reactive;
using System.Reactive.Linq;
using System.Threading.Tasks;

namespace VanishBox.ViewModels
{
    public class MainWindowViewModel : ViewModelBase
    {
        private string _infoText;
        /// <summary>
        /// Текст выводимый в окне.
        /// </summary>
        public string InfoText
        {
            get => _infoText;
            set => this.RaiseAndSetIfChanged(ref _infoText, value);
        }

        private bool _showProgressControls;
        /// <summary>
        /// Отображать компоненты информирующее о прогрессе выполняемой операции.
        /// </summary>
        public bool ShowProgressControls
        {
            get => _showProgressControls;
            set => this.RaiseAndSetIfChanged(ref _showProgressControls, value);
        }

        private string _progressText;
        /// <summary>
        /// Текст с информацией о текущей операции.
        /// </summary>
        public string ProgressText
        {
            get => _progressText;
            set => this.RaiseAndSetIfChanged(ref _progressText, value);
        }

        private int _progressValue;
        /// <summary>
        /// Значение прогресса 0-100%.
        /// </summary>
        public int ProgressValue
        {
            get => _progressValue;
            set => this.RaiseAndSetIfChanged(ref _progressValue, value);
        }

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
        /// О программе.
        /// </summary>
        public ReactiveCommand<Unit, Unit> AboutProgramCommand { get; }

        /// <summary>
        /// Закрыть окно.
        /// </summary>
        public ReactiveCommand<Unit, object> ExitCommand { get; }

        /// <summary>
        /// Зашифровать.
        /// </summary>
        public ReactiveCommand<Unit, Unit> CryptCommand { get; }

        /// <summary>
        /// Расшифровать.
        /// </summary>
        public ReactiveCommand<Unit, Unit> DecryptCommand { get; }

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

        /// <summary>
        /// Окно "О программе".
        /// </summary>
        public Interaction<AboutProgramViewModel, Unit> AboutProgramDialog { get; }

        private string[] _selectedFiles { get; set; }
        
        public MainWindowViewModel()
        {
            SelectFilesDialog = new Interaction<object, string[]?>();
            ResetSettingsDialog = new Interaction<string, bool>();
            ShowKeyParamDialog = new Interaction<PasswordInputWindowViewModel, UserKeyViewModel?>();
            AboutProgramDialog = new Interaction<AboutProgramViewModel, Unit>();

            ExitCommand = ReactiveCommand.Create(() => new object());
            CryptCommand = ReactiveCommand.CreateFromTask(async () =>
            {
                await CryptFiles();
            });

            DecryptCommand = ReactiveCommand.CreateFromTask(async () =>
            {
                await DecryptFiles();
            });

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

            AboutProgramCommand = ReactiveCommand.CreateFromTask(async () =>
            {
                await AboutProgramDialog.Handle(new AboutProgramViewModel());
            });

            SetUserInfoText();
        }

        /// <summary>
        /// Выбрать файлы которые требуется зашифровать/расшифровать.
        /// </summary>
        /// <returns></returns>
        public async Task SelectFiles()
        {
            _selectedFiles = await SelectFilesDialog.Handle(new object());
            
            foreach (var item in _selectedFiles)
            {
                AppendInfoText(item);
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

            AppendInfoText("Настройки сброшены.");
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

             //Тест.
            AppendInfoText(result.Password);
            AppendInfoText(result.Path);
        }

        /// <summary>
        /// Задает справочный текст в информационном окне.
        /// </summary>
        private void SetUserInfoText()
        {
            string infoText = "Для начала работы в меню \"Файл\" выберите \"Открыть\", выделите файлы требующие обработки.";
            AppendInfoText(infoText);
        }

        /// <summary>
        /// Выводит текст в информационное окно.
        /// </summary>
        /// <param name="text"></param>
        private void AppendInfoText(string text)
        {
            InfoText += $"{text}\r\n";
        }

        /// <summary>
        /// Шифрует  все файлы.
        /// </summary>
        /// <returns></returns>
        private async Task CryptFiles()
        {
            ShowProgressControls = true;

            while (ProgressValue < 100)
            {
                ProgressText = $"Выполненно {ProgressValue}%";
                await Task.Delay(700);
                ProgressValue++;
            }
        }

        /// <summary>
        /// Расшифровать все файлы.
        /// </summary>
        /// <returns></returns>
        private async Task DecryptFiles()
        {
            ShowProgressControls = true;
        }
    }
}